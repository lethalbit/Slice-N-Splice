/* elf.hh - ELF structures and utilities */
#pragma once
#if !defined(__SNS_ELF_HH__)
#define __SNS_ELF_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <iostream>
/* I know this is my code, but shh */
#include <span.hh>

#include <utility.hh>
#include <mmap_t.hh>
#include <fd_t.hh>
#include <zlib.hh>

#if defined(CXXFS_EXP)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

/* Basic types used to composite the larger types */

/* 32-Bit ELF types */
using elf32_addr_t   = /* ALIGN(0x04) */ uint32_t;
using elf32_off_t    = /* ALIGN(0x04) */ uint32_t;
using elf32_half_t   = /* ALIGN(0x02) */ uint16_t;
using elf32_word_t   = /* ALIGN(0x04) */ uint32_t;
using elf32_xword_t  = /* ALIGN(0x08) */ uint64_t;
using elf32_sword_t  = /* ALIGN(0x04) */ int32_t;
using elf32_sxword_t = /* ALIGN(0x08) */ int64_t;
using elf32_uchar_t  = /* ALIGN(0x01) */ uint8_t;

/* 64-Bit ELF Types */
using elf64_addr_t   = /* ALIGN(0x08) */ uint64_t;
using elf64_off_t    = /* ALIGN(0x08) */ uint64_t;
using elf64_half_t   = /* ALIGN(0x02) */ uint16_t;
using elf64_word_t   = /* ALIGN(0x04) */ uint32_t;
using elf64_xword_t  = /* ALIGN(0x08) */ uint64_t;
using elf64_sword_t  = /* ALIGN(0x04) */ int32_t;
using elf64_sxword_t = /* ALIGN(0x08) */ int64_t;
using elf64_uchar_t  = /* ALIGN(0x01) */ uint8_t;

/* Alternative type declarations */
struct elf_types_32_t;
struct elf_types_64_t;

/* ELF Class */
enum class elf_class_t : uint8_t {
	None  = 0x00U,
	ELF32 = 0x01U,
	ELF64 = 0x02U,
};
extern const std::array<const enum_pair_t<elf_class_t>, 3> elf_class_s;
extern std::ostream& operator<<(std::ostream& out, const elf_class_t& eclass);

/* ELF Data encoding */
enum class elf_data_t : uint8_t {
	None = 0x00U,
	LSB  = 0x01U,
	MSB  = 0x02U,
};
extern const std::array<const enum_pair_t<elf_data_t>, 3> elf_data_s;
extern std::ostream& operator<<(std::ostream& out, const elf_data_t& data);


/* ELF Ident Version */
enum class elf_ident_version_t : uint8_t {
	None    = 0x00U,
	Current = 0x01U,
};
extern const std::array<const enum_pair_t<elf_ident_version_t>, 2> elf_ident_version_s;
extern std::ostream& operator<<(std::ostream& out, const elf_ident_version_t& ident_version);


/* ELF Operating System ABI */
enum class elf_osabi_t : uint8_t {
	SystemV       = 0x00U,
	HPUX          = 0x01U,
	NetBSD        = 0x02U,
	Linux         = 0x03U,
	GNUMach       = 0x04U,    /* IA64 Supplemented */
	IA32          = 0x05U,    /* IA64 Supplemented */
	Solaris       = 0x06U,
	AIX           = 0x07U,
	IRIX          = 0x08U,
	FreeBSD       = 0x09U,
	TRU64         = 0x0AU,
	Modesto       = 0x0BU,
	OpenBSD       = 0x0CU,
	OpenVMS       = 0x0DU,
	NonStopKernel = 0x0EU,
	AmigaROS      = 0x0FU,
	FenixOS       = 0x10U,
	OpenVOS       = 0x11U,
	ArmAEABI      = 0x40U,
	PIC           = 0x52U,
	Arm           = 0x61U,
	Standalone    = 0xFFU,
};
extern const std::array<const enum_pair_t<elf_osabi_t>, 22> elf_osabi_s;
extern std::ostream& operator<<(std::ostream& out, const elf_osabi_t& abi);


/* ELF Version */
enum class elf_version_t : uint32_t {
	None    = 0x00U,
	Current = 0x01U,
};
extern const std::array<const enum_pair_t<elf_version_t>, 2> elf_version_s;
extern std::ostream& operator<<(std::ostream& out, const elf_version_t& version);


/* ELF Object type */
enum class ALIGN(0x02) elf_type_t : uint16_t {
	None         = 0x0000U,
	Relocatable  = 0x0001U,
	Executable   = 0x0002U,
	SharedObject = 0x0003U,
	CoreFile     = 0x0004U,
	/* Tandem is naughty and didn't put their shit in the OS reserved section */
	TandemPIC	 = 0x0064U,
	TandemHybrid = 0x00C8U,
	LOW_OS       = 0xFE00U,
	/* Operating System Specific */
	HIGH_OS      = 0xFEFFU,
	LOW_PROC     = 0xFF00U,
	/* Processor Specific */
	HIGH_PROC    = 0xFFFFU,
};
extern const std::array<const enum_pair_t<elf_type_t>, 11> elf_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_type_t& type);


/* ELF Machine Types */
enum class ALIGN(0x02) elf_machine_t : uint16_t {
	None          = 0U,     /* No machine */
	M32           = 1U,     /* AT&T WE 32100 */
	SPARC         = 2U,     /* SUN SPARC */
	I386          = 3U,     /* Intel 80386 */
	M68K          = 4U,     /* Motorola m68k family */
	M88K          = 5U,     /* Motorola m88k family */
	IAMCU         = 6U,     /* Intel MCU */
	I860          = 7U,     /* Intel 80860 */
	MIPS          = 8U,     /* MIPS R3000 big-endian */
	S370          = 9U,     /* IBM System/370 */
	MIPS_RS3_LE   = 10U,    /* MIPS R3000 little-endian */
	/* reserved 11-14 */
	PARISC        = 15U,    /* HPPA */
	/* reserved 16 */
	VPP500        = 17U,    /* Fujitsu VPP500 */
	SPARC32PLUS   = 18U,    /* Sun's "v8plus" */
	I960          = 19U,    /* Intel 80960 */
	PPC           = 20U,    /* PowerPC */
	PPC64         = 21U,    /* PowerPC 64-bit */
	S390          = 22U,    /* IBM S390 */
	SPU           = 23U,    /* IBM SPU/SPC */
	/* reserved 24-35 */
	V800          = 36U,    /* NEC V800 series */
	FR20          = 37U,    /* Fujitsu FR20 */
	RH32          = 38U,    /* TRW RH-32 */
	RCE           = 39U,    /* Motorola RCE */
	ARM           = 40U,    /* ARM */
	FAKE_ALPHA    = 41U,    /* Digital Alpha */
	SH            = 42U,    /* Hitachi SH */
	SPARCV9       = 43U,    /* SPARC v9 64-bit */
	TRICORE       = 44U,    /* Siemens Tricore */
	ARC           = 45U,    /* Argonaut RISC Core */
	H8_300        = 46U,    /* Hitachi H8/300 */
	H8_300H       = 47U,    /* Hitachi H8/300H */
	H8S           = 48U,    /* Hitachi H8S */
	H8_500        = 49U,    /* Hitachi H8/500 */
	IA_64         = 50U,    /* Intel Merced */
	MIPS_X        = 51U,    /* Stanford MIPS-X */
	COLDFIRE      = 52U,    /* Motorola Coldfire */
	M68HC12       = 53U,    /* Motorola M68HC12 */
	MMA           = 54U,    /* Fujitsu MMA Multimedia Accelerator */
	PCP           = 55U,    /* Siemens PCP */
	NCPU          = 56U,    /* Sony nCPU embeeded RISC */
	NDR1          = 57U,    /* Denso NDR1 microprocessor */
	STARCORE      = 58U,    /* Motorola Start*Core processor */
	ME16          = 59U,    /* Toyota ME16 processor */
	ST100         = 60U,    /* STMicroelectronic ST100 processor */
	TINYJ         = 61U,    /* Advanced Logic Corp. Tinyj emb.fam */
	X86_64        = 62U,    /* AMD x86-64 architecture */
	PDSP          = 63U,    /* Sony DSP Processor */
	PDP10         = 64U,    /* Digital PDP-10 */
	PDP11         = 65U,    /* Digital PDP-11 */
	FX66          = 66U,    /* Siemens FX66 microcontroller */
	ST9PLUS       = 67U,    /* STMicroelectronics ST9+ 8/16 mc */
	ST7           = 68U,    /* STmicroelectronics ST7 8 bit mc */
	M68HC16       = 69U,    /* Motorola MC68HC16 microcontroller */
	M68HC11       = 70U,    /* Motorola MC68HC11 microcontroller */
	M68HC08       = 71U,    /* Motorola MC68HC08 microcontroller */
	M68HC05       = 72U,    /* Motorola MC68HC05 microcontroller */
	SVX           = 73U,    /* Silicon Graphics SVx */
	ST19          = 74U,    /* STMicroelectronics ST19 8 bit mc */
	VAX           = 75U,    /* Digital VAX */
	CRIS          = 76U,    /* Axis Communications 32-bit emb.proc */
	JAVELIN       = 77U,    /* Infineon Technologies 32-bit emb.proc */
	FIREPATH      = 78U,    /* Element 14 64-bit DSP Processor */
	ZSP           = 79U,    /* LSI Logic 16-bit DSP Processor */
	MMIX          = 80U,    /* Donald Knuth's educational 64-bit proc */
	HUANY         = 81U,    /* Harvard University machine-independent object files */
	PRISM         = 82U,    /* SiTera Prism */
	AVR           = 83U,    /* Atmel AVR 8-bit microcontroller */
	FR30          = 84U,    /* Fujitsu FR30 */
	D10V          = 85U,    /* Mitsubishi D10V */
	D30V          = 86U,    /* Mitsubishi D30V */
	V850          = 87U,    /* NEC v850 */
	M32R          = 88U,    /* Mitsubishi M32R */
	MN10300       = 89U,    /* Matsushita MN10300 */
	MN10200       = 90U,    /* Matsushita MN10200 */
	PJ            = 91U,    /* picoJava */
	OPENRISC      = 92U,    /* OpenRISC 32-bit embedded processor */
	ARC_COMPACT   = 93U,    /* ARC International ARCompact */
	XTENSA        = 94U,    /* Tensilica Xtensa Architecture */
	VIDEOCORE     = 95U,    /* Alphamosaic VideoCore */
	TMM_GPP       = 96U,    /* Thompson Multimedia General Purpose Proc */
	NS32K         = 97U,    /* National Semi. 32000 */
	TPC           = 98U,    /* Tenor Network TPC */
	SNP1K         = 99U,    /* Trebia SNP 1000 */
	ST200         = 100U,   /* STMicroelectronics ST200 */
	IP2K          = 101U,   /* Ubicom IP2xxx */
	MAX           = 102U,   /* MAX processor */
	CR            = 103U,   /* National Semi. CompactRISC */
	F2MC16        = 104U,   /* Fujitsu F2MC16 */
	MSP430        = 105U,   /* Texas Instruments msp430 */
	BLACKFIN      = 106U,   /* Analog Devices Blackfin DSP */
	SE_C33        = 107U,   /* Seiko Epson S1C33 family */
	SEP           = 108U,   /* Sharp embedded microprocessor */
	ARCA          = 109U,   /* Arca RISC */
	UNICORE       = 110U,   /* PKU-Unity & MPRC Peking Uni. mc series */
	EXCESS        = 111U,   /* eXcess configurable cpu */
	DXP           = 112U,   /* Icera Semi. Deep Execution Processor */
	ALTERA_NIOS2  = 113U,   /* Altera Nios II */
	CRX           = 114U,   /* National Semi. CompactRISC CRX */
	XGATE         = 115U,   /* Motorola XGATE */
	C166          = 116U,   /* Infineon C16x/XC16x */
	M16C          = 117U,   /* Renesas M16C */
	DSPIC30F      = 118U,   /* Microchip Technology dsPIC30F */
	CE            = 119U,   /* Freescale Communication Engine RISC */
	M32C          = 120U,   /* Renesas M32C */
	/* reserved 121-130 */
	TSK3000       = 131U,   /* Altium TSK3000 */
	RS08          = 132U,   /* Freescale RS08 */
	SHARC         = 133U,   /* Analog Devices SHARC family */
	ECOG2         = 134U,   /* Cyan Technology eCOG2 */
	SCORE7        = 135U,   /* Sunplus S+core7 RISC */
	DSP24         = 136U,   /* New Japan Radio (NJR) 24-bit DSP */
	VIDEOCORE3    = 137U,   /* Broadcom VideoCore III */
	LATTICEMICO32 = 138U,   /* RISC for Lattice FPGA */
	SE_C17        = 139U,   /* Seiko Epson C17 */
	TI_C6000      = 140U,   /* Texas Instruments TMS320C6000 DSP */
	TI_C2000      = 141U,   /* Texas Instruments TMS320C2000 DSP */
	TI_C5500      = 142U,   /* Texas Instruments TMS320C55x DSP */
	TI_ARP32      = 143U,   /* Texas Instruments App. Specific RISC */
	TI_PRU        = 144U,   /* Texas Instruments Prog. Realtime Unit */
	/* reserved 145-159 */
	MMDSP_PLUS    = 160U,   /* STMicroelectronics 64bit VLIW DSP */
	CYPRESS_M8C   = 161U,   /* Cypress M8C */
	R32C          = 162U,   /* Renesas R32C */
	TRIMEDIA      = 163U,   /* NXP Semi. TriMedia */
	QDSP6         = 164U,   /* QUALCOMM DSP6 */
	I8051         = 165U,   /* Intel 8051 and variants */
	STXP7X        = 166U,   /* STMicroelectronics STxP7x */
	NDS32         = 167U,   /* Andes Tech. compact code emb. RISC */
	ECOG1X        = 168U,   /* Cyan Technology eCOG1X */
	MAXQ30        = 169U,   /* Dallas Semi. MAXQ30 mc */
	XIMO16        = 170U,   /* New Japan Radio (NJR) 16-bit DSP */
	MANIK         = 171U,   /* M2000 Reconfigurable RISC */
	CRAYNV2       = 172U,   /* Cray NV2 vector architecture */
	RX            = 173U,   /* Renesas RX */
	METAG         = 174U,   /* Imagination Tech. META */
	MCST_ELBRUS   = 175U,   /* MCST Elbrus */
	ECOG16        = 176U,   /* Cyan Technology eCOG16 */
	CR16          = 177U,   /* National Semi. CompactRISC CR16 */
	ETPU          = 178U,   /* Freescale Extended Time Processing Unit */
	SLE9X         = 179U,   /* Infineon Tech. SLE9X */
	L10M          = 180U,   /* Intel L10M */
	K10M          = 181U,   /* Intel K10M */
	/* reserved 182 */
	AARCH64       = 183U,   /* ARM AARCH64 */
	/* reserved 184 */
	AVR32         = 185U,   /* Amtel 32-bit microprocessor */
	STM8          = 186U,   /* STMicroelectronics STM8 */
	TILE64        = 187U,   /* Tileta TILE64 */
	TILEPRO       = 188U,   /* Tilera TILEPro */
	MICROBLAZE    = 189U,   /* Xilinx MicroBlaze */
	CUDA          = 190U,   /* NVIDIA CUDA */
	TILEGX        = 191U,   /* Tilera TILE-Gx */
	CLOUDSHIELD   = 192U,   /* CloudShield */
	COREA_1ST     = 193U,   /* KIPO-KAIST Core-A 1st gen. */
	COREA_2ND     = 194U,   /* KIPO-KAIST Core-A 2nd gen. */
	ARC_COMPACT2  = 195U,   /* Synopsys ARCompact V2 */
	OPEN8         = 196U,   /* Open8 RISC */
	RL78          = 197U,   /* Renesas RL78 */
	VIDEOCORE5    = 198U,   /* Broadcom VideoCore V */
	R78KOR        = 199U,   /* Renesas 78KOR */
	F56800EX      = 200U,   /* Freescale 56800EX DSC */
	BA1           = 201U,   /* Beyond BA1 */
	BA2           = 202U,   /* Beyond BA2 */
	XCORE         = 203U,   /* XMOS xCORE */
	MCHP_PIC      = 204U,   /* Microchip 8-bit PIC(r) */
	/* reserved 205-209 */
	KM32          = 210U,   /* KM211 KM32 */
	KMX32         = 211U,   /* KM211 KMX32 */
	EMX16         = 212U,   /* KM211 KMX16 */
	EMX8          = 213U,   /* KM211 KMX8 */
	KVARC         = 214U,   /* KM211 KVARC */
	CDP           = 215U,   /* Paneve CDP */
	COGE          = 216U,   /* Cognitive Smart Memory Processor */
	COOL          = 217U,   /* Bluechip CoolEngine */
	NORC          = 218U,   /* Nanoradio Optimized RISC */
	CSR_KALIMBA   = 219U,   /* CSR Kalimba */
	Z80           = 220U,   /* Zilog Z80 */
	VISIUM        = 221U,   /* Controls and Data Services VISIUMcore */
	FT32          = 222U,   /* FTDI Chip FT32 */
	MOXIE         = 223U,   /* Moxie processor */
	AMDGPU        = 224U,   /* AMD GPU */
	/* reserved 225-242 */
	RISCV        = 243U,    /* RISC-V */
	BPF          = 247U,    /* Linux BPF -- in-kernel virtual machine */
	/* Unofficial */
	BONELESS     = 0x666U,  /* whitequark's Boneless-III CPU https://github.com/whitequark/Boneless-CPU */
	ALPHA        = 0x9026U, /* Digital Alpha */
};
extern const std::array<const enum_pair_t<elf_machine_t>, 180> elf_machine_s;
extern std::ostream& operator<<(std::ostream& out, const elf_machine_t& machine);


/* ELF Header Flags */
enum class elf_flag_t : uint32_t {
	None              = 0x00000000U,
	/* ARM Processor Supplement */
	ARM_HasEntry      = 0x00000002U,
	ARM_SymsAreSrtd   = 0x00000004U,
	ARM_DynSymsSegIdx = 0x00000008U,
	ARM_MapSymsFirst  = 0x00000010U,
	ARM_EABIMask      = 0xFF000000U,

	/* IA64 Processor Supplement */
	IA64_ReduceDFP    = 0x00000020U,
	IA64_ConsGP       = 0x00000040U,
	IA64_Nofuncdesc   = 0x00000080U,
	IA64_Absolute     = 0x00000100U,
	IA64_MaskOS       = 0x00FF000FU,
	IA64_Arch         = 0xFF000000U,

	/* Tandem / NSK Supplement */

};
template<>
struct EnableBitmask<elf_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_flag_t>, 12> elf_flag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_flag_t& flag);


/* ELF Special Section Indexes */
enum class elf_shns_t : uint16_t {
	Undefined   = 0x0000U,
	LowReserve  = 0xFF00U,
	LowProc     = 0xFF00U,
	Before      = 0xFF00U,
	After       = 0xFF01U,
	HighProc    = 0xFF1FU,
	LowOS       = 0xFF20U,
	HighOS      = 0xFF3FU,
	ABS         = 0xFFF1U,
	Common      = 0xFFF2U,
	XIndex      = 0xFFFFU,
	HighReserve = 0xFFFFU,
};
extern const std::array<const enum_pair_t<elf_shns_t>, 12> elf_shns_s;
extern std::ostream& operator<<(std::ostream& out, const elf_shns_t& shns);


/* ELF section header types */
enum class elf_shtype_t : uint32_t {
	Null             = 0x00000000U,
	ProgBits         = 0x00000001U,
	SymbolTable      = 0x00000002U,
	StringTable      = 0x00000003U,
	RelA             = 0x00000004U,
	HashTable        = 0x00000005U,
	Dynamic          = 0x00000006U,
	Note             = 0x00000007U,
	NoBits           = 0x00000008U,
	Rel              = 0x00000009U,
	SHLib            = 0x0000000AU,
	DynamicSymbols   = 0x0000000BU,
	InitArray        = 0x0000000EU,
	FiniArray        = 0x0000000FU,
	PreinitArray     = 0x00000010U,
	Group            = 0x00000011U,
	SymbolTableIndex = 0x00000012U,
	LowOS            = 0x60000000U,
	GNUAttributes    = 0x6FFFFFF5U,
	GNUHash          = 0x6FFFFFF6U,
	GNULibList       = 0x6FFFFFF7U,
	Checksum         = 0x6FFFFFF8U,
	SUNMove          = 0x6FFFFFFAU,
	SUNComDat        = 0x6FFFFFFBU,
	SUNSymInfo       = 0x6FFFFFFCU,
	GNUVerDef        = 0x6FFFFFFDU,
	GNUVerNeed       = 0x6FFFFFFEU,
	GNUVerSym        = 0x6FFFFFFFU,
	HighOS           = 0x6FFFFFFFU,
	LowProc          = 0x70000000U,
	HighProc         = 0x7FFFFFFFU,
	LowUser          = 0x80000000U,
	HighUser         = 0xFFFFFFFFU,
};
extern const std::array<const enum_pair_t<elf_shtype_t>, 33> elf_shtype_s;
extern std::ostream& operator<<(std::ostream& out, const elf_shtype_t& type);


/* 32-Bit Section flags */
enum class elf32_shflags_t : uint32_t {
	None            = 0x00000000U,
	Write           = 0x00000001U,
	Alloc           = 0x00000002U,
	ExecInstr       = 0x00000004U,
	Merge           = 0x00000010U,
	Strings         = 0x00000020U,
	InfoLink        = 0x00000040U,
	LinkOrder       = 0x00000080U,
	NonconformingOS = 0x00000100U,
	Group           = 0x00000200U,
	TLS             = 0x00000400U,
	Compressed      = 0x00000800U,
	Ordered         = 0x40000000U,
	Exclude         = 0x80000000U,
};
template<>
struct EnableBitmask<elf32_shflags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf32_shflags_t>, 14> elf32_shflags_s;
extern std::ostream& operator<<(std::ostream& out, const elf32_shflags_t& shflag);


/* 64-Bit Section flags */
enum class elf64_shflags_t : uint64_t {
	None            = 0x00000000U,
	Write           = 0x00000001U,
	Alloc           = 0x00000002U,
	ExecInstr       = 0x00000004U,
	Merge           = 0x00000010U,
	Strings         = 0x00000020U,
	InfoLink        = 0x00000040U,
	LinkOrder       = 0x00000080U,
	NonconformingOS = 0x00000100U,
	Group           = 0x00000200U,
	TLS             = 0x00000400U,
	Compressed      = 0x00000800U,
	Ordered         = 0x40000000U,
	Exclude         = 0x80000000U,
};
template<>
struct EnableBitmask<elf64_shflags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf64_shflags_t>, 14> elf64_shflags_s;
extern std::ostream& operator<<(std::ostream& out, const elf64_shflags_t& shflag);


/* Section group flags */
enum class elf_shgroup_t : uint32_t {
	None     = 0x00000000U,
	Comdat   = 0x00000001U,
	MaskOS   = 0x0FF00000U,
	MaskProc = 0xF0000000U,
};
template<>
struct EnableBitmask<elf_shgroup_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_shgroup_t>, 4> elf_shgroup_s;
extern std::ostream& operator<<(std::ostream& out, const elf_shgroup_t& shgroup);


/* Symbol Binding */
enum class elf_symbol_binding_t : uint8_t {
	Local    = 0x00U,
	Global   = 0x01U,
	Weak     = 0x02U,
	LowOS    = 0x0AU,
	HighOS   = 0x0CU,
	LowProc  = 0x0DU,
	HighProc = 0x0FU,
};
extern const std::array<const enum_pair_t<elf_symbol_binding_t>, 7> elf_symbol_binding_s;
extern std::ostream& operator<<(std::ostream& out, const elf_symbol_binding_t& symbind);


/* Symbol Type */
enum class elf_symbol_type_t : uint8_t {
	NoType             = 0x00U,
	Object             = 0x01U,
	Function           = 0x02U,
	Section            = 0x03U,
	File               = 0x04U,
	Common             = 0x05U,
	ThreadLocalStorage = 0x06U,
	LowOS              = 0x0AU,
	HighOS             = 0x0CU,
	LowProc            = 0x0DU,
	HighProc           = 0x0FU,
};
extern const std::array<const enum_pair_t<elf_symbol_type_t>, 11> elf_symbol_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_symbol_type_t& symtype);


/* Symbol Visibility */
enum class elf_symbol_visibility_t : uint8_t {
	Default   = 0x00U,
	Internal  = 0x01U,
	Hidden    = 0x02U,
	Protected = 0x03U,
};
extern const std::array<const enum_pair_t<elf_symbol_visibility_t>, 4> elf_symbol_visibility_s;
extern std::ostream& operator<<(std::ostream& out, const elf_symbol_visibility_t& symvis);


/* Symbol binding */
enum  class elf_syminfo_bound_t : uint16_t {
	None       = 0x0000U,
	LowReserve = 0xFF00U,
	Parent     = 0xFFFEU,
	Self       = 0xFFFFU,
};
extern const std::array<const enum_pair_t<elf_syminfo_bound_t>, 4> elf_syminfo_bound_s;
extern std::ostream& operator<<(std::ostream& out, const elf_syminfo_bound_t& symibind);


/* Symbol info flags */
enum class elf_syminfo_flag_t : uint16_t {
	None     = 0x0000U,
	Direct   = 0x0001U,
	Passthru = 0x0002U,
	Copy     = 0x0004U,
	LazyLoad = 0x0008U,
};
template<>
struct EnableBitmask<elf_syminfo_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_syminfo_flag_t>, 5> elf_syminfo_flag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_syminfo_flag_t& symiflag);


/* Program header type */
enum class elf_phdr_type_t : uint32_t {
	None               = 0x00000000U,
	Load               = 0x00000001U,
	Dynamic            = 0x00000002U,
	Interpreter        = 0x00000003U,
	Note               = 0x00000004U,
	SHLib              = 0x00000005U,
	ProgramHeader      = 0x00000006U,
	ThreadLocalStorage = 0x00000007U,
	LowOS              = 0x60000000U,
	GNUEHFrame         = 0x6474E550U,
	GNUStack           = 0x6474E551U,
	GNURelRO           = 0x6474E552U,
	SUNBSS             = 0x6FFFFFFAU,
	SUNStack           = 0x6FFFFFFBU,
	HighOS             = 0x6FFFFFFFU,
	LowProc            = 0x70000000U,
	HighProc           = 0x7FFFFFFFU,
};
extern const std::array<const enum_pair_t<elf_phdr_type_t>, 17> elf_phdr_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_phdr_type_t& phdrtype);


/* Program header flags */
enum class elf_phdr_flags_t : uint32_t {
	None     = 0x00000000U,
	Execute  = 0x00000001U,
	Write    = 0x00000002U,
	Read     = 0x00000004U,
	MaskOS   = 0x0FF00000U,
	MaskProc = 0xF0000000U,
};
template<>
struct EnableBitmask<elf_phdr_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_phdr_flags_t>, 6> elf_phdr_flags_s;
extern std::ostream& operator<<(std::ostream& out, const elf_phdr_flags_t& phdrflag);


/* Note Segment Descriptor Types */
enum class elf_note_desc_t : uint32_t {
	None            = 0x00000000U,
	PRSTATUS        = 0x00000001U,
	FPREGSET        = 0x00000002U,
	PRPSINFO        = 0x00000003U,
	PRXREG          = 0x00000004U,
	TASKSTRUCT      = 0x00000005U,
	PLATFORM        = 0x00000006U,
	AUXV            = 0x00000007U,
	GWINDOWS        = 0x00000008U,
	ASRS            = 0x00000009U,
	PSTATUS         = 0x0000000AU,
	PSINFO          = 0x0000000DU,
	PRCRED          = 0x0000000EU,
	UTSNAME         = 0x0000000FU,
	LWPSTATUS       = 0x00000010U,
	LWPSINFO        = 0x00000011U,
	PRFPXREG        = 0x00000014U,
	/* Power PC Specific */
	PPC_VMV         = 0x00000100U,
	PPC_SPE         = 0x00000101U,
	PPC_VSX         = 0x00000102U,
	PPC_TAR         = 0x00000103U,
	PPC_PPR         = 0x00000104U,
	PPC_DSCR        = 0x00000105U,
	PPC_EBB         = 0x00000106U,
	PPC_PMU         = 0x00000107U,
	PPC_TM_CGPR     = 0x00000108U,
	PPC_TM_CFPR     = 0x00000109U,
	PPC_TM_CVMX     = 0x0000010AU,
	PPC_TM_CVSX     = 0x0000010BU,
	PPC_TM_SPR      = 0x0000010CU,
	PPC_TM_CTAR     = 0x0000010DU,
	PPC_TM_CPPR     = 0x0000010EU,
	PPC_TM_CDSCR    = 0x0000010FU,
	PPC_PKEY        = 0x00000110U,
	/* x86 PC Specific */
	I386_TLS        = 0x00000200U,
	I386_IOPerm     = 0x00000201U,
	X86_XState      = 0x00000202U,
	/* System/390 Specific */
	S390_HighGprs   = 0x00000300U,
	S390_Timer      = 0x00000301U,
	S390_TODCmp     = 0x00000302U,
	S390_TODPreg    = 0x00000303U,
	S390_CTRS       = 0x00000304U,
	S390_Prefix     = 0x00000305U,
	S390_LastBreak  = 0x00000306U,
	S390_SystemCall = 0x00000307U,
	S390_TBD        = 0x00000308U,
	S390_VXRS_Low   = 0x00000309U,
	S390_VXRS_High  = 0x0000030AU,
	S390_GS_CB      = 0x0000030BU,
	S390_GS_BC      = 0x0000030CU,
	S390_RI_CB      = 0x0000030DU,
	/* ARM Specific */
	ARM_VFP         = 0x00000400U,
	ARM_TLS         = 0x00000401U,
	ARM_HW_BREAK    = 0x00000402U,
	ARM_HW_WATCH    = 0x00000403U,
	ARM_SYSTEM_CALL = 0x00000404U,
	ARM_SVE         = 0x00000405U,
	/* Extended */
	File            = 0x46494C45U,
	PRXFPReg        = 0x46E62B7FU,
	SigInfo         = 0x53494749U,
};
extern const std::array<const enum_pair_t<elf_note_desc_t>, 60> elf_note_desc_s;
extern std::ostream& operator<<(std::ostream& out, const elf_note_desc_t& notedesc);


/* Dynamic section tags */
enum class elf32_dyn_tag_t : elf32_sword_t {
	None             = 0x00000000,
	Needed           = 0x00000001,
	PLTRelSize       = 0x00000002,
	PLTGOT           = 0x00000003,
	Hash             = 0x00000004,
	StrTab           = 0x00000005,
	SymTab           = 0x00000006,
	RelA             = 0x00000006,
	RelASize         = 0x00000008,
	RelAEnt          = 0x00000009,
	StrTabSize       = 0x0000000A,
	SymTabEnt        = 0x0000000B,
	Init             = 0x0000000C,
	Fini             = 0x0000000D,
	SOName           = 0x0000000E,
	RPath            = 0x0000000F,
	Symbolic         = 0x00000010,
	Rel              = 0x00000011,
	RelSize          = 0x00000012,
	RelEnt           = 0x00000013,
	PLTRel           = 0x00000014,
	Debug            = 0x00000015,
	TextRel          = 0x00000016,
	JmpRel           = 0x00000017,
	BindNow          = 0x00000018,
	InitArray        = 0x00000019,
	FiniArray        = 0x0000001A,
	InitArraySize    = 0x0000001B,
	FiniArraySize    = 0x0000001C,
	RunPath          = 0x0000001D,
	Flags            = 0x0000001E,
	Encoding         = 0x0000001F,
	PreInitArray     = 0x00000020,
	PreInitArraySize = 0x00000021,
	SymtabSHNDX      = 0x00000022,
	LowOS            = 0x6000000D,
	HighOS           = 0x6FFFF000,
	GNUPrelinked     = 0x6FFFFDF5,
	GNUConflictSize  = 0x6FFFFDF6,
	GNULibListSize   = 0x6FFFFDF7,
	Checksum         = 0x6FFFFDF8,
	PLTPadSize       = 0x6FFFFDF9,
	MoveEnt          = 0x6FFFFDFA,
	MoveSize         = 0x6FFFFDFB,
	Feature          = 0x6FFFFDFC,
	PosFlag          = 0x6FFFFDFD,
	SyminfoSize      = 0x6FFFFDFE,
	SyminfoEnt       = 0x6FFFFDFF,
	AddrRNGLow       = 0x6FFFFE00,
	GNUHash          = 0x6FFFFEF5,
	TLSDescPLT       = 0x6FFFFEF6,
	TLSDescGOT       = 0x6FFFFEF7,
	GNUConflict      = 0x6FFFFEF8,
	GNULibList       = 0x6FFFFEF9,
	Config           = 0x6FFFFEFA,
	DEPAudit         = 0x6FFFFEFB,
	Audit            = 0x6FFFFEFC,
	PLTPad           = 0x6FFFFEFD,
	MoveTAB          = 0x6FFFFEFE,
	Syminfo          = 0x6FFFFEFF,
	VerSym           = 0x6FFFFF00,
	RelACount        = 0x6FFFFFF9,
	RelCount         = 0x6FFFFFFA,
	Flags_1          = 0x6FFFFFFB,
	VerDef           = 0x6FFFFFFC,
	VerDefNum        = 0x6FFFFFFD,
	VerNeed          = 0x6FFFFFFE,
	VerNeedNum       = 0x6FFFFFFF,
	LowProc          = 0x70000000,
	Auxiliary        = 0x7FFFFFFD,
	HighProc         = 0x7FFFFFFF,
};
extern const std::array<const enum_pair_t<elf32_dyn_tag_t>, 71> elf32_dyn_tag_s;
extern std::ostream& operator<<(std::ostream& out, const elf32_dyn_tag_t& dyntag);


enum class elf64_dyn_tag_t : elf64_sxword_t {
	None             = 0x00000000,
	Needed           = 0x00000001,
	PLTRelSize       = 0x00000002,
	PLTGOT           = 0x00000003,
	Hash             = 0x00000004,
	StrTab           = 0x00000005,
	SymTab           = 0x00000006,
	RelA             = 0x00000006,
	RelASize         = 0x00000008,
	RelAEnt          = 0x00000009,
	StrTabSize       = 0x0000000A,
	SymTabEnt        = 0x0000000B,
	Init             = 0x0000000C,
	Fini             = 0x0000000D,
	SOName           = 0x0000000E,
	RPath            = 0x0000000F,
	Symbolic         = 0x00000010,
	Rel              = 0x00000011,
	RelSize          = 0x00000012,
	RelEnt           = 0x00000013,
	PLTRel           = 0x00000014,
	Debug            = 0x00000015,
	TextRel          = 0x00000016,
	JmpRel           = 0x00000017,
	BindNow          = 0x00000018,
	InitArray        = 0x00000019,
	FiniArray        = 0x0000001A,
	InitArraySize    = 0x0000001B,
	FiniArraySize    = 0x0000001C,
	RunPath          = 0x0000001D,
	Flags            = 0x0000001E,
	Encoding         = 0x0000001F,
	PreInitArray     = 0x00000020,
	PreInitArraySize = 0x00000021,
	SymtabSHNDX      = 0x00000022,
	LowOS            = 0x6000000D,
	HighOS           = 0x6FFFF000,
	GNUPrelinked     = 0x6FFFFDF5,
	GNUConflictSize  = 0x6FFFFDF6,
	GNULibListSize   = 0x6FFFFDF7,
	Checksum         = 0x6FFFFDF8,
	PLTPadSize       = 0x6FFFFDF9,
	MoveEnt          = 0x6FFFFDFA,
	MoveSize         = 0x6FFFFDFB,
	Feature          = 0x6FFFFDFC,
	PosFlag          = 0x6FFFFDFD,
	SyminfoSize      = 0x6FFFFDFE,
	SyminfoEnt       = 0x6FFFFDFF,
	AddrRNGLow       = 0x6FFFFE00,
	GNUHash          = 0x6FFFFEF5,
	TLSDescPLT       = 0x6FFFFEF6,
	TLSDescGOT       = 0x6FFFFEF7,
	GNUConflict      = 0x6FFFFEF8,
	GNULibList       = 0x6FFFFEF9,
	Config           = 0x6FFFFEFA,
	DEPAudit         = 0x6FFFFEFB,
	Audit            = 0x6FFFFEFC,
	PLTPad           = 0x6FFFFEFD,
	MoveTAB          = 0x6FFFFEFE,
	Syminfo          = 0x6FFFFEFF,
	VerSym           = 0x6FFFFF00,
	RelACount        = 0x6FFFFFF9,
	RelCount         = 0x6FFFFFFA,
	Flags_1          = 0x6FFFFFFB,
	VerDef           = 0x6FFFFFFC,
	VerDefNum        = 0x6FFFFFFD,
	VerNeed          = 0x6FFFFFFE,
	VerNeedNum       = 0x6FFFFFFF,
	LowProc          = 0x70000000,
	Auxiliary        = 0x7FFFFFFD,
	HighProc         = 0x7FFFFFFF,
};
extern const std::array<const enum_pair_t<elf64_dyn_tag_t>, 71> elf64_dyn_tag_s;
extern std::ostream& operator<<(std::ostream& out, const elf64_dyn_tag_t& dyntag);


/* Dynamic Entry Flags */
enum class elf_dyn_flags_t : uint8_t {
	None      = 0x00U,
	Origin    = 0x01U,
	Symbolic  = 0x02U,
	TextRel   = 0x04U,
	BindNow   = 0x08U,
	StaticTLS = 0x10U,
};
template<>
struct EnableBitmask<elf_dyn_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_flags_t>, 6> elf_dyn_flags_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_flags_t& dynflag);


/* Extended Dynamic Entry Flags */
enum class elf_dyn_eflags_t : uint32_t {
	None           = 0x00000000U,
	Now            = 0x00000001U,
	Global         = 0x00000002U,
	Group          = 0x00000004U,
	NoDelete       = 0x00000008U,
	LoadFLTR       = 0x00000010U,
	InitFirst      = 0x00000020U,
	NoOpen         = 0x00000040U,
	Origin         = 0x00000080U,
	Direct         = 0x00000100U,
	Trans          = 0x00000200U,
	Interpose      = 0x00000400U,
	NoDefaultLib   = 0x00000800U,
	NoDump         = 0x00001000U,
	ConFALT        = 0x00002000U,
	EndFileTEE     = 0x00004000U,
	DispRelDNE     = 0x00008000U,
	DispRelPND     = 0x00010000U,
	NoDirect       = 0x00020000U,
	IgnoreMultiDef = 0x00040000U,
	NoKSyms        = 0x00080000U,
	NoHDR          = 0x00100000U,
	Edited         = 0x00200000U,
	NoReloc        = 0x00400000U,
	SymInterpose   = 0x00800000U,
	GlobalAudit    = 0x01000000U,
	Singleton      = 0x02000000U,
	Stub           = 0x04000000U,
	PIE            = 0x08000000U,
};
template<>
struct EnableBitmask<elf_dyn_eflags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_eflags_t>, 29> elf_dyn_eflags_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_eflags_t& dyneflag);


/* Dynamic Table Features */
enum class elf_dyn_feature_t : uint32_t {
	None    = 0x00U,
	ParInit = 0x01U,
	ConfEXP = 0x02U,
};
template<>
struct EnableBitmask<elf_dyn_feature_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_feature_t>, 3> elf_dyn_feature_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_feature_t& dynfeat);


/* Dynamic entry posflags */
enum class elf_dyn_posflag_t : uint32_t {
	None      = 0x00U,
	LazyLoad  = 0x01U,
	GroupPerm = 0x02U,
};
template<>
struct EnableBitmask<elf_dyn_posflag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_posflag_t>, 3> elf_dyn_posflag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_posflag_t& dynposf);


/* Compressed Header Type */
enum class elf_chdr_type_t : uint32_t {
	None     = 0x00000000U,
	Zlib     = 0x00000001U,
	LowOS    = 0x60000000U,
	HighOS   = 0x6fffffffU,
	LowProc  = 0x70000000U,
	HighProc = 0x7fffffffU,
};
extern const std::array<const enum_pair_t<elf_chdr_type_t>, 6> elf_chdr_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_chdr_type_t& chdrtype);

/* Version definition revisions */
enum class elf_verdef_revision_t : uint16_t {
	None    = 0x0000U,
	Current = 0x0001U,
};
extern const std::array<const enum_pair_t<elf_verdef_revision_t>, 2> elf_verdef_revision_s;
extern std::ostream& operator<<(std::ostream& out, const elf_verdef_revision_t& verdefr);


/* Version definitions flags */
enum class elf_verdef_flag_t : uint16_t {
	None = 0x0000U,
	Base = 0x0001U,
	Weak = 0x0002U,
};
template<>
struct EnableBitmask<elf_verdef_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_verdef_flag_t>, 3> elf_verdef_flag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_verdef_flag_t& verdeff);


/* Versym index values */
enum class elf_verdef_index_t : uint16_t {
	Local      = 0x0000U,
	Global     = 0x0000U,
	LowReserve = 0xFF00U,
	Eliminate  = 0xFF01U,
};
extern const std::array<const enum_pair_t<elf_verdef_index_t>, 4> elf_verdef_index_s;
extern std::ostream& operator<<(std::ostream& out, const elf_verdef_index_t& verdefi);


/* Vernaux flags */
enum class elf_vernaux_flag_t : uint16_t {
	None = 0x00U,
	Weak = 0x02U,
};
template<>
struct EnableBitmask<elf_vernaux_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_vernaux_flag_t>, 2> elf32_vernaux_s;
extern std::ostream& operator<<(std::ostream& out, const elf_vernaux_flag_t& verauxf);


/* Auxiliary Vector Types */
enum elf_auxv_type_t : uint32_t {
	Null                  = 0x00000000U,
	Ignore                = 0x00000001U,
	ExecFD                = 0x00000002U,
	PHDR                  = 0x00000003U,
	PHEnt                 = 0x00000004U,
	PHNum                 = 0x00000005U,
	PageSize              = 0x00000006U,
	Base                  = 0x00000007U,
	Entry                 = 0x00000008U,
	NotElf                = 0x00000009U,
	UID                   = 0x0000000AU,
	EUID                  = 0x0000000BU,
	GID                   = 0x0000000CU,
	EGID                  = 0x0000000DU,
	Platform              = 0x0000000EU,
	HWCap                 = 0x0000000FU,
	ClkTck                = 0x00000010U,
	FPUControlWord        = 0x00000011U,
	DataCacheBSize        = 0x00000012U,
	InstructionCacheBSize = 0x00000013U,
	UnifiedCacheBSize     = 0x00000014U,
	IgnorePPC             = 0x00000015U, /* poor PPC */
	Secure                = 0x00000016U,
	BasePlatform          = 0x00000017U,
	Random                = 0x00000018U,
	HWCap2                = 0x00000019U, /* The electric boogaloo */
	ExecFN                = 0x0000001AU,
	Sysinfo               = 0x0000001BU,
	SysinfoEHDR           = 0x0000001CU,
	L1InstCacheShape      = 0x0000001DU,
	L1DataCacheShape      = 0x0000001EU,
	L2CacheShape          = 0x0000001FU,
	L3CacheShape          = 0x00000020U,
	L1InstCacheSize       = 0x00000021U,
	L1InstCacheGeometry   = 0x00000022U,
	L1DataCacheSize       = 0x00000023U,
	L1DataCacheGeometry   = 0x00000024U,
	L2CacheSize           = 0x00000025U,
	L2CacheGeometry       = 0x00000026U,
	L3CacheSize           = 0x00000027U,
	L3CacheGeometry       = 0x00000028U,
};
extern const std::array<const enum_pair_t<elf_auxv_type_t>, 41> elf_auxv_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_auxv_type_t& auxvtype);


/* Note known systems */
enum class elf_note_os_t : uint32_t {
	Linux    = 0x00000000U,
	GNU      = 0x00000001U,
	Solaris2 = 0x00000002U,
	FreeBSD  = 0x00000003U,
};
extern const std::array<const enum_pair_t<elf_note_os_t>, 4> elf_note_os_s;
extern std::ostream& operator<<(std::ostream& out, const elf_note_os_t& noteos);


/* Note Type */
enum class elf_note_type_t : uint32_t {
	None            = 0x00000000U,
	GNUABI          = 0x00000001U,
	GNUHWCap        = 0x00000002U,
	GNUBuildID      = 0x00000003U,
	GNUGoldVersion  = 0x00000004U,
	GNUPropertyType = 0x00000005U,
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


	[[nodiscard]]
	bool is_valid() const noexcept {
		return (_magic1 == 0x7FU && _magic2 == 0x45U
			 && _magic3 == 0x4CU && _magic4 == 0x46U);
	}

	void set() noexcept {
		_magic1 = 0x7FU;
		_magic2 = 0x45U;
		_magic3 = 0x4CU;
		_magic4 = 0x46U;
	}

	[[nodiscard]]
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

	[[nodiscard]]
	elf_magic_t magic() const noexcept { return _magic; }

	void eclass(const elf_class_t eclass) noexcept { _eclass = eclass; }
	[[nodiscard]]
	elf_class_t eclass() const noexcept { return _eclass; }

	void data(const elf_data_t data) noexcept { _data = data; }
	[[nodiscard]]
	elf_data_t data() const noexcept { return _data; }

	void version(const elf_ident_version_t version) noexcept { _version = version; }
	[[nodiscard]]
	elf_ident_version_t version() const noexcept { return _version; }

	void abi(const elf_osabi_t abi) noexcept { _abi = abi; }
	[[nodiscard]]
	elf_osabi_t abi() const noexcept { return _abi; }

	void abi_version(const uint8_t abi_version) noexcept { _abi_version = abi_version; }
	[[nodiscard]]
	uint8_t abi_version() const noexcept { return _abi_version; }

	void padding(const std::array<uint8_t, 7> padding) noexcept { _padding = padding; }
	[[nodiscard]]
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
	elf_flag_t _flags;      /* Processor dependent flags */
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
	[[nodiscard]]
	elf_ident_t ident() const noexcept { return _ident; }

	void type(const elf_type_t type) noexcept { _type = type; }
	[[nodiscard]]
	elf_type_t type() const noexcept { return _type; }

	void machine(const elf_machine_t machine) noexcept { _machine = machine; }
	[[nodiscard]]
	elf_machine_t machine() const noexcept { return _machine; }

	void version(const elf_version_t version) noexcept { _version = version; }
	[[nodiscard]]
	elf_version_t version() const noexcept { return _version; }

	void entry(const addr_t entry) noexcept { _entry = entry; }
	[[nodiscard]]
	addr_t entry() const noexcept { return _entry; }

	void phoff(const offset_t phoff) noexcept { _phoff = phoff; }
	[[nodiscard]]
	offset_t phoff() const noexcept { return _phoff; }

	void shoff(const offset_t shoff) noexcept { _shoff = shoff; }
	[[nodiscard]]
	offset_t shoff() const noexcept { return _shoff; }

	void flags(const elf_flag_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	elf_flag_t flags() const noexcept { return _flags; }

	void ehsize(const half_t ehsize) noexcept { _ehsize = ehsize; }
	[[nodiscard]]
	half_t ehsize() const noexcept { return _ehsize; }

	void phentsize(const half_t phentsize) noexcept { _phentsize = phentsize; }
	[[nodiscard]]
	half_t phentsize() const noexcept { return _phentsize; }

	void phnum(const half_t phnum) noexcept { _phnum = phnum; }
	[[nodiscard]]
	half_t phnum() const noexcept { return _phnum; }

	void shentsize(const half_t shentsize) noexcept { _shentsize = shentsize; }
	[[nodiscard]]
	half_t shentsize() const noexcept { return _shentsize; }

	void shnum(const half_t shnum) noexcept { _shnum = shnum; }
	[[nodiscard]]
	half_t shnum() const noexcept { return _shnum; }

	void shstrndx(const half_t shstrndx) noexcept { _shstrndx = shstrndx; }
	[[nodiscard]]
	half_t shstrndx() const noexcept { return _shstrndx; }
};
using elf32_ehdr_t = elf_ehdr_t<elf_types_32_t>;
using elf64_ehdr_t = elf_ehdr_t<elf_types_64_t>;



/* ELF Section Header */
template<typename T>
struct elf_shdr_t final {
	using addr_t    = typename T::addr_t;
	using offset_t  = typename T::offset_t;
	using shflags_t = typename T::shflags_t;
	using word_t    = typename T::word_t;
	using xword_t   = typename T::xword_t;
private:
	word_t _name;
	elf_shtype_t _type;
	shflags_t _flags;
	addr_t _addr;
	offset_t _offset;
	xword_t _size;
	word_t _link;
	word_t _info;
	xword_t _addraline;
	xword_t _entsize;
public:
	constexpr elf_shdr_t() noexcept :
		_name{}, _type{}, _flags{}, _addr{}, _offset{}, _size{},
		_link{}, _info{}, _addraline{}, _entsize{} { /* NOP */ }

	elf_shdr_t(word_t name, elf_shtype_t type, shflags_t flags,
			addr_t addr, offset_t offset, xword_t size,
			word_t link, word_t info, xword_t addraline,
			xword_t entsize) noexcept :
		_name{name}, _type{type}, _flags{flags}, _addr{addr}, _offset{offset},
		_size{size}, _link{link}, _info{info}, _addraline{addraline},
		_entsize{entsize} { /* NOP */}

	void name(const word_t name) noexcept { _name = name; }
	[[nodiscard]]
	word_t name() const noexcept { return _name; }

	void type(const elf_shtype_t type) noexcept { _type = type; }
	[[nodiscard]]
	elf_shtype_t type() const noexcept { return _type; }

	void flags(const shflags_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	shflags_t flags() const noexcept { return _flags; }

	void addr(const addr_t addr) noexcept { _addr = addr; }
	[[nodiscard]]
	addr_t addr() const noexcept { return _addr; }

	void offset(const offset_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	offset_t offset() const noexcept { return _offset; }

	void size(const xword_t size) noexcept { _size = size; }
	[[nodiscard]]
	xword_t size() const noexcept { return _size; }

	void link(const word_t link) noexcept { _link = link; }
	[[nodiscard]]
	word_t link() const noexcept { return _link; }

	void info(const word_t info) noexcept { _info = info; }
	[[nodiscard]]
	word_t info() const noexcept { return _info; }

	void addraline(const xword_t addraline) noexcept { _addraline = addraline; }
	[[nodiscard]]
	xword_t addraline() const noexcept { return _addraline; }

	void entsize(const xword_t entsize) noexcept { _entsize = entsize; }
	[[nodiscard]]
	xword_t entsize() const noexcept { return _entsize; }
};

using elf32_shdr_t = elf_shdr_t<elf_types_32_t>;
using elf64_shdr_t = elf_shdr_t<elf_types_32_t>;


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
	[[nodiscard]]
	elf32_word_t name() const noexcept { return _name; }

	void value(const elf32_addr_t value) noexcept { _value = value; }
	[[nodiscard]]
	elf32_addr_t value() const noexcept { return _value; }

	void size(const elf32_word_t size) noexcept { _size = size; }
	[[nodiscard]]
	elf32_word_t size() const noexcept { return _size; }

	void info(const elf32_uchar_t info) noexcept { _info = info; }
	[[nodiscard]]
	elf32_uchar_t info() const noexcept { return _info; }

	void other(const elf32_uchar_t other) noexcept { _other = other; }
	[[nodiscard]]
	elf32_uchar_t other() const noexcept { return _other; }

	void shndx(const elf32_half_t shndx) noexcept { _shndx = shndx; }
	[[nodiscard]]
	elf32_half_t shndx() const noexcept { return _shndx; }

	[[nodiscard]]
	elf32_uchar_t bind() const noexcept { return _info >> 0x04U; }
	[[nodiscard]]
	elf32_uchar_t type() const noexcept { return _info &  0x0FU; }
	[[nodiscard]]
	elf32_uchar_t visibility() const noexcept { return _other &  0x03U; }

	static elf32_uchar_t make_info(elf32_uchar_t bind,
								   elf32_uchar_t type) {
		return (bind << 0x04U) + (type & 0xFU);
	}
};

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
	[[nodiscard]]
	elf64_word_t name() const noexcept { return _name; }

	void info(const elf64_uchar_t info) noexcept { _info = info; }
	[[nodiscard]]
	elf64_uchar_t info() const noexcept { return _info; }

	void other(const elf64_uchar_t other) noexcept { _other = other; }
	[[nodiscard]]
	elf64_uchar_t other() const noexcept { return _other; }

	void shndx(const elf64_half_t shndx) noexcept { _shndx = shndx; }
	[[nodiscard]]
	elf64_half_t shndx() const noexcept { return _shndx; }

	void value(const elf64_addr_t value) noexcept { _value = value; }
	[[nodiscard]]
	elf64_addr_t value() const noexcept { return _value; }

	void size(const elf64_xword_t size) noexcept { _size = size; }
	[[nodiscard]]
	elf64_xword_t size() const noexcept { return _size; }

	[[nodiscard]]
	elf64_uchar_t bind() const noexcept { return _info >> 0x04U; }
	[[nodiscard]]
	elf64_uchar_t type() const noexcept { return _info &  0x0FU; }
	[[nodiscard]]
	elf64_uchar_t visibility() const noexcept { return _other &  0x03U; }

	static elf64_uchar_t make_info(elf64_uchar_t bind,
								   elf64_uchar_t type) {
		return (bind << 0x04U) + (type & 0xFU);
	}
};

/* ELF syminfo section */
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
	[[nodiscard]]
	elf_syminfo_bound_t boundto() const noexcept { return _boundto; }

	void flags(elf_syminfo_flag_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	elf_syminfo_flag_t flags() const noexcept { return _flags; }
};

/* ELF Relocation information */
template<typename T>
struct elf_rel_t final {
	using addr_t  = typename T::addr_t;
	using xword_t = typename T::xword_t;
private:
	addr_t _offset;
	xword_t _info;
public:
	constexpr elf_rel_t() noexcept :
		_offset{}, _info{} { /* NOP */ }

	elf_rel_t(addr_t offset, xword_t info) noexcept :
		_offset{offset}, _info{info} { /* NOP */ }

	void offset(const addr_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	addr_t offset() const noexcept { return _offset; }

	void info(const xword_t info) noexcept { _info = info; }
	[[nodiscard]]
	xword_t info() const noexcept { return _info; }
};

using elf32_rel_t = elf_rel_t<elf_types_32_t>;
using elf64_rel_t = elf_rel_t<elf_types_64_t>;

/* ELF Relocation information with addend */
template<typename T>
struct elf_rela_t final {
	using addr_t    = typename T::addr_t;
	using xword_t   = typename T::xword_t;
	using sword_t   = typename T::sword_t;
private:
	addr_t _offset;
	xword_t _info;
	sword_t _addend;
public:
	constexpr elf_rela_t() noexcept :
		_offset{}, _info{}, _addend{} { /* NOP */ }

	elf_rela_t(addr_t offset, xword_t info,	sword_t addend) noexcept :
		_offset{offset}, _info{info}, _addend{addend} { /* NOP */ }

	void offset(const addr_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	addr_t offset() const noexcept { return _offset; }

	void info(const xword_t info) noexcept { _info = info; }
	[[nodiscard]]
	xword_t info() const noexcept { return _info; }

	void addend(const sword_t addend) noexcept { _addend = addend; }
	[[nodiscard]]
	sword_t addend() const noexcept { return _addend; }


	[[nodiscard]]
	xword_t sym() const noexcept { return (_info >> T::sym_shift); }

	[[nodiscard]]
	xword_t type() const noexcept { return (_info & 0xFFFFFFFFUL); }

	static xword_t make_info(xword_t sym, xword_t type) {
		return (sym << T::sym_shift) + (type & 0xFFFFFFFFUL);
	}
};

using elf32_rela_t = elf_rela_t<elf_types_32_t>;
using elf64_rela_t = elf_rela_t<elf_types_64_t>;


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
	[[nodiscard]]
	elf_phdr_type_t type() const noexcept { return _type; }

	void flags(const elf_phdr_flags_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	elf_phdr_flags_t flags() const noexcept { return _flags; }

	void offset(const elf32_off_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	elf32_off_t offset() const noexcept { return _offset; }

	void vaddr(const elf32_addr_t vaddr) noexcept { _vaddr = vaddr; }
	[[nodiscard]]
	elf32_addr_t vaddr() const noexcept { return _vaddr; }

	void paddr(const elf32_addr_t paddr) noexcept { _paddr = paddr; }
	[[nodiscard]]
	elf32_addr_t paddr() const noexcept { return _paddr; }

	void filesz(const elf32_word_t filesz) noexcept { _filesz = filesz; }
	[[nodiscard]]
	elf32_word_t filesz() const noexcept { return _filesz; }

	void memsize(const elf32_word_t memsize) noexcept { _memsize = memsize; }
	[[nodiscard]]
	elf32_word_t memsize() const noexcept { return _memsize; }

	void align(const elf32_word_t align) noexcept { _align = align; }
	[[nodiscard]]
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
	[[nodiscard]]
	elf_phdr_type_t type() const noexcept { return _type; }

	void flags(const elf_phdr_flags_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	elf_phdr_flags_t flags() const noexcept { return _flags; }

	void offset(const elf64_off_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	elf64_off_t offset() const noexcept { return _offset; }

	void vaddr(const elf64_addr_t vaddr) noexcept { _vaddr = vaddr; }
	[[nodiscard]]
	elf64_addr_t vaddr() const noexcept { return _vaddr; }

	void paddr(const elf64_addr_t paddr) noexcept { _paddr = paddr; }
	[[nodiscard]]
	elf64_addr_t paddr() const noexcept { return _paddr; }

	void filesz(const elf64_xword_t filesz) noexcept { _filesz = filesz; }
	[[nodiscard]]
	elf64_xword_t filesz() const noexcept { return _filesz; }

	void memsize(const elf64_xword_t memsize) noexcept { _memsize = memsize; }
	[[nodiscard]]
	elf64_xword_t memsize() const noexcept { return _memsize; }

	void align(const elf64_xword_t align) noexcept { _align = align; }
	[[nodiscard]]
	elf64_xword_t align() const noexcept { return _align; }
};

/* ELF Dynamic Section */
template<typename T>
struct elf_dyn_t final {
	using dyn_tag_t = typename T::dyn_tag_t;
	using xword_t   = typename T::xword_t;
	using addr_t    = typename T::addr_t;
private:
	dyn_tag_t _tag;
	union {
		xword_t value;
		addr_t pointer;
	} _data;
public:
	constexpr elf_dyn_t() noexcept :
		_tag{}, _data{} { /* NOP */ }

	elf_dyn_t(dyn_tag_t tag, xword_t value) noexcept :
		_tag{tag}, _data{value} { /* NOP */ }

	void tag(dyn_tag_t tag) noexcept { _tag = tag; }
	[[nodiscard]]
	dyn_tag_t tag() const noexcept { return _tag; }

	void value(xword_t value) noexcept { _data.value = value; }
	[[nodiscard]]
	xword_t value() const noexcept { return _data.value; }

	void pointer(addr_t pointer) noexcept { _data.pointer = pointer; }
	[[nodiscard]]
	addr_t pointer() const noexcept { return _data.pointer; }
};

using elf32_dyn_t = elf_dyn_t<elf_types_32_t>;
using elf64_dyn_t = elf_dyn_t<elf_types_64_t>;


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
	[[nodiscard]]
	elf_chdr_type_t type() const noexcept { return _type; }

	void size(const elf32_word_t size) noexcept { _size = size; }
	[[nodiscard]]
	elf32_word_t size() const noexcept { return _size; }

	void addr_align(const elf32_word_t addr_align) noexcept { _addr_align = addr_align;	}
	[[nodiscard]]
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
	[[nodiscard]]
	elf_chdr_type_t type() const noexcept { return _type; }

	void reserved(const elf64_word_t reserved) noexcept { _reserved = reserved; }
	[[nodiscard]]
	elf64_word_t reserved() const noexcept { return _reserved; }

	void size(const elf64_xword_t size) noexcept { _size = size; }
	[[nodiscard]]
	elf64_xword_t size() const noexcept { return _size; }

	void addr_align(const elf64_xword_t addr_align) noexcept { _addr_align = addr_align; }
	[[nodiscard]]
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
	[[nodiscard]]
	elf_verdef_revision_t version() const noexcept { return _version; }

	void flags(const elf_verdef_flag_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	elf_verdef_flag_t flags() const noexcept { return _flags; }

	void index(const elf_verdef_index_t index) noexcept { _index = index; }
	[[nodiscard]]
	elf_verdef_index_t index() const noexcept { return _index; }

	void count(const half_t count) noexcept { _count = count; }
	[[nodiscard]]
	half_t count() const noexcept { return _count; }

	void hash(const word_t hash) noexcept { _hash = hash; }
	[[nodiscard]]
	word_t hash() const noexcept { return _hash; }

	void aux_offset(const word_t aux_offset) noexcept { _aux_offset = aux_offset; }
	[[nodiscard]]
	word_t aux_offset() const noexcept { return _aux_offset; }

	void next_offset(const word_t next_offset) noexcept { _next_offset = next_offset; }
	[[nodiscard]]
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
	[[nodiscard]]
	word_t name() const noexcept { return _name; }

	void next(const word_t next) noexcept { _next = next; }
	[[nodiscard]]
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
	[[nodiscard]]
	half_t version() const noexcept { return _version; }

	void count(const half_t count) noexcept { _count = count; }
	[[nodiscard]]
	half_t count() const noexcept { return _count; }

	void file(const word_t file) noexcept { _file = file; }
	[[nodiscard]]
	word_t file() const noexcept { return _file; }

	void aux(const word_t aux) noexcept { _aux = aux; }
	[[nodiscard]]
	word_t aux() const noexcept { return _aux; }

	void next(const word_t next) noexcept { _next = next; }
	[[nodiscard]]
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
	[[nodiscard]]
	word_t hash() const noexcept { return _hash; }

	void flags(const elf_vernaux_flag_t flags) noexcept { _flags = flags; }
	[[nodiscard]]
	elf_vernaux_flag_t flags() const noexcept { return _flags; }

	void other(const half_t other) noexcept { _other = other; }
	[[nodiscard]]
	half_t other() const noexcept { return _other; }

	void name(const word_t name) noexcept { _name = name; }
	[[nodiscard]]
	word_t name() const noexcept { return _name; }

	void next(const word_t next) noexcept { _next = next; }
	[[nodiscard]]
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
	[[nodiscard]]
	xword_t type() const noexcept { return _type; }

	void value(const xword_t value) noexcept { _data.value = value; }
	[[nodiscard]]
	xword_t value() const noexcept { return _data.value; }

	void pointer(const addr_t pointer) noexcept { _data.pointer = pointer; }
	[[nodiscard]]
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
		_name_sz{name_sz}, _desc_sz{desc_sz}, _type{type} { /* NOP */ }

	void name_sz(const word_t name_sz) noexcept { _name_sz = name_sz; }
	[[nodiscard]]
	word_t name_sz() const noexcept { return _name_sz; }

	void desc_sz(const word_t desc_sz) noexcept { _desc_sz = desc_sz; }
	[[nodiscard]]
	word_t desc_sz() const noexcept { return _desc_sz; }

	void type(const word_t type) noexcept { _type = type; }
	[[nodiscard]]
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
	[[nodiscard]]
	elf32_xword_t value() const noexcept { return _value; }

	void info(const elf32_word_t info) noexcept { _info = info; }
	[[nodiscard]]
	elf32_word_t info() const noexcept { return _info; }

	void offset(const elf32_off_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	elf32_off_t offset() const noexcept { return _offset; }

	void repeat(const elf32_half_t repeat) noexcept { _repeat = repeat; }
	[[nodiscard]]
	elf32_half_t repeat() const noexcept { return _repeat; }

	void stride(const elf32_half_t stride) noexcept { _stride = stride; }
	[[nodiscard]]
	elf32_half_t stride() const noexcept { return _stride; }


	[[nodiscard]]
	uint8_t size() const noexcept { return _info; }
	[[nodiscard]]
	elf32_word_t sym() const noexcept { return _info >> 0x8U; }

	static elf32_word_t make_info(const elf32_word_t sym,
								   const uint8_t size) {
		return (sym << 0x08U) + size;
	}
};

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
	[[nodiscard]]
	elf64_xword_t value() const noexcept { return _value; }

	void info(const elf64_xword_t info) noexcept { _info = info; }
	[[nodiscard]]
	elf64_xword_t info() const noexcept { return _info; }

	void offset(const elf64_off_t offset) noexcept { _offset = offset; }
	[[nodiscard]]
	elf64_off_t offset() const noexcept { return _offset; }

	void repeat(const elf64_half_t repeat) noexcept { _repeat = repeat; }
	[[nodiscard]]
	elf64_half_t repeat() const noexcept { return _repeat; }

	void stride(const elf64_half_t stride) noexcept { _stride = stride; }
	[[nodiscard]]
	elf64_half_t stride() const noexcept { return _stride; }


	[[nodiscard]]
	uint8_t size() const noexcept { return _info; }
	[[nodiscard]]
	elf64_xword_t sym() const noexcept { return _info >> 0x8U; }

	static elf64_xword_t make_info(const elf64_xword_t sym,
								   const uint8_t size) {
		return (sym << 0x08U) + size;
	}

};

/* ELF Type definitions */
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

	/* Flags / Enums */
	using shflags_t = elf32_shflags_t;
	using dyn_tag_t = elf32_dyn_tag_t;


	/* Constants */
	constexpr static uint8_t sym_shift = 0x08U;
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

	/* Flags / Enums */
	using shflags_t = elf64_shflags_t;
	using dyn_tag_t = elf64_dyn_tag_t;

	/* Constants */
	constexpr static uint8_t sym_shift = 0x20U;
};

/* ELF File representation itself */
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
	fs::path _file;         /* The path to the object file */
	fd_t _file_fd;          /* File descriptor */
	mmap_t _file_map;       /* mmap object for object file */
	ehdr_t _header;         /* The executable header */
	span<phdr_t> _pheaders; /* Program Headers */
	span<shdr_t> _sheaders; /* Section Headers */
	char* _strtbl;          /* Section name string table */

	bool _constructed;
public:
	constexpr elf_t() noexcept :
		_file{}, _file_fd{}, _file_map{}, _header{}, _pheaders{}, _sheaders{},
		_strtbl{}, _constructed{true} { /* NOP */ }

	elf_t(fs::path file, bool readonly = true) noexcept :
		_file{std::move(file)}, _file_fd{_file.c_str(), O_RDONLY},
		_file_map{_file_fd.map(PROT_READ)},
		_header{}, _pheaders{}, _sheaders{}, _strtbl{}, _constructed{true} {

		if(!_file_map.valid()) {
			_constructed = false;
			return;
		} else {
			_header = _file_map.at<ehdr_t>(0);

			if(_header.phnum() > 0 && _header.phoff() < _file_map.length())
				_pheaders = {&_file_map.at<phdr_t>(_header.phoff()), _header.phnum()};


			if(_header.shnum() > 0 && _header.shoff() < _file_map.length()) {
				_sheaders = {&_file_map.at<shdr_t>(_header.shoff()), _header.shnum()};
				/* Map the string table */
				if(_header.shstrndx() < _file_map.length()) {
					auto strtbl_offset =  (_sheaders[_header.shstrndx()]).offset();
					if(strtbl_offset < _file_map.length())
						_strtbl = &_file_map.at<char>(strtbl_offset);
				}
			}
		}
	}

	bool valid() const noexcept { return _constructed; }
	bool elf_valid() const noexcept { return _header.ident().magic().is_valid(); }

	void header(const ehdr_t header) noexcept { _header = header; }
	[[nodiscard]]
	ehdr_t header() const noexcept { return _header; }

	void pheaders(const span<phdr_t> pheaders) noexcept { _pheaders = pheaders; }
	[[nodiscard]]
	span<phdr_t> pheaders() const noexcept { return _pheaders; }

	void sheaders(const span<shdr_t> sheaders) noexcept { _sheaders = sheaders; }
	[[nodiscard]]
	span<shdr_t> sheaders() const noexcept { return _sheaders; }


	std::string section_name(const size_t index) const noexcept { return std::string(_strtbl + index); }

};
using elf32_t = elf_t<elf_types_32_t>;
using elf64_t = elf_t<elf_types_64_t>;

uint32_t elf_hash(const uint8_t* name);


#endif /* __SNS_ELF_HH__ */

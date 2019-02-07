/* macho.hh - Mach-O structures and utilities */
#pragma once
#if !defined(__SNS_MACHO_HH__)
#define __SNS_MACHO_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <iostream>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <utility.hh>

/* Basic types used to composite the larger types */

/* Mach-O CPU Type */
enum class mach_cpu_t : uint32_t {
	Any = -1U,
	None = 0,
	Vax = 1,
	MC680X0 = 6,
	x86,
	x86_64 = (mach_cpu_t::x86 | 0x01000000),
	MIPS = 8,
	MC98000 = 10,
	HPPA,
	ARM,
	ARM64 = (mach_cpu_t::ARM | 0x01000000),
	MC88000 = 13,
	SPARC,
	I860,
	ALPHA,
	PowerPC = 18,
	PowerPC64 = (mach_cpu_t::PowerPC | 0x01000000)
};
extern const std::array<const enum_pair_t<mach_cpu_t>, 17> mach_cpu_s;
extern std::ostream& operator<<(std::ostream& out, const mach_cpu_t& mcpu);

/* Mach-O CPU Sub-type */
enum class mach_cpu_sub_t : uint32_t {
	Multiple = -1U,
	LSB = 0,
	MSB,
};
extern const std::array<const enum_pair_t<mach_cpu_sub_t>, 3> mach_cpu_sub_s;
extern std::ostream& operator<<(std::ostream& out, const mach_cpu_sub_t& mcpusub);

/* Mach-O CPU Sub-type */
enum class mach_filetype_t : uint32_t {
	None = 0,
	Object,
	Execute,
	FVMLib,
	Core,
	Preload,
	Dylib,
	Dylinker,
	Bundle,
	DylibStub,
	DSym,
	KEXTBundle,
};
extern const std::array<const enum_pair_t<mach_filetype_t>, 17> mach_filetype_s;
extern std::ostream& operator<<(std::ostream& out, const mach_filetype_t& mftype);


enum class mach_flags_t : uint32_t {
	None = 0,
	NoUndefs = 0x00000001,
	IncrLink = 0x00000002,
	DyndLink = 0x00000004,
	BinDataLoad = 0x00000008,
	PreBound = 0x00000010,
	SplitSegs = 0x00000020,
	LazyInit = 0x00000040,
	TwoLevel = 0x00000080,
	ForceFlat = 0x00000100,
	NoMultiDefs = 0x00000200,
	NoFixePreBinding = 0x00000400,
	Prebindable = 0x000000800,
	AllModsBound = 0x00001000,
	SubsectionsViaSyms = 0x00002000,
	Canonical = 0x00004000,
	WeakDefines = 0x00008000,
	BindsToWeak = 0x00010000,
	AllowStackExecution = 0x00020000,
	RootSafe = 0x00040000,
	SetUIDSafe = 0x00080000,
	NoReexportedDylibs = 0x00100000,
	PIE = 0x00200000,
	DeadStrippableDylib = 0x00400000,
	HasTLVDescriptions = 0x00800000,
	NoHeapExecution = 0x01000000,
	AppExtensionSafe = 0x02000000,
};
template<>
struct EnableBitmask<mach_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<mach_flags_t>, 27> mach_flags_s;
extern std::ostream& operator<<(std::ostream& out, const mach_flags_t& mflag);


/* Mach-O Structure definitions */

/* Mach-O Magic */
struct mach_magic_t final {
private:
	uint8_t _magic1;
	uint8_t _magic2;
	uint8_t _magic3;
	uint8_t _magic4;
public:
	constexpr mach_magic_t() noexcept :
		_magic1{}, _magic2{}, _magic3{}, _magic4{} { /* NOP */ }

	bool is_valid() const  noexcept {
		return (_magic1 == 0xFE && _magic2 == 0xED
			 && _magic3 == 0xFA && _magic4 == 0xCE);
	}

	void set() noexcept {
		_magic1 = 0xFE;
		_magic2 = 0xED;
		_magic3 = 0xFA;
		_magic4 = 0xCE;
	}

	std::array<uint8_t, 4> get() const noexcept {
		return { _magic1, _magic2, _magic3, _magic4 };
	}
};


/* 32-Bit Mach-O header */
struct mach32_header_t final {
private:
	uint32_t _magic;        /* Should always be 0xFEEDFACE */
	mach_cpu_t _cpu;        /* CPU Type */
	mach_cpu_sub_t _subcpu; /* Sub CPU Type */
	uint32_t _filetype;     /* Type of binary file this is */
	uint32_t _ncmds;        /* Number of load commands */
	uint32_t _cmd_size;     /* The size of each load command */
	mach_flags_t _flags;    /* Various flags */
public:
	constexpr mach32_header_t() noexcept :
		_magic{}, _cpu{}, _subcpu{}, _filetype{}, _ncmds{}, _cmd_size{},
		_flags{} { /* NOP */ }
};



#endif /* __SNS_MACHO_HH__ */

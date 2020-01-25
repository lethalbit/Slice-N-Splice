/* macho.hh - Mach-O structures and utilities */
#pragma once
#if !defined(__SNS_MACHO_HH__)
#define __SNS_MACHO_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <iostream>
#include <filesystem>

#include <mmap_t.hh>
#include <fd_t.hh>
#include <utility.hh>

namespace fs = std::filesystem;


/* Basic types used to composite the larger types */

/* Mach-O CPU Type */
enum class mach_cpu_t : uint32_t {
	Any       = 0xFFFFFFFFU,
	None      = 0x00000000U,
	Vax       = 0x00000001U,
	MC680X0   = 0x00000006U,
	x86       = 0x00000007U,
	x86_64    = (mach_cpu_t::x86 | 0x01000000U),
	MIPS      = 0x00000008U,
	MC98000   = 0x0000000AU,
	HPPA      = 0x0000000BU,
	ARM       = 0x0000000CU,
	ARM64     = (mach_cpu_t::ARM | 0x01000000U),
	MC88000   = 0x0000000DU,
	SPARC     = 0x0000000EU,
	I860      = 0x0000000FU,
	ALPHA     = 0x00000010U,
	PowerPC   = 0x00000012U,
	PowerPC64 = (mach_cpu_t::PowerPC | 0x01000000U)
};
extern const std::array<const enum_pair_t<mach_cpu_t>, 17> mach_cpu_s;
extern std::ostream& operator<<(std::ostream& out, const mach_cpu_t& mcpu);

/* Mach-O CPU Sub-type */
enum class mach_cpu_sub_t : uint32_t {
	Multiple = 0xFFFFFFFFU,
	LSB      = 0x00000000U,
	MSB      = 0x00000001U,
};
extern const std::array<const enum_pair_t<mach_cpu_sub_t>, 3> mach_cpu_sub_s;
extern std::ostream& operator<<(std::ostream& out, const mach_cpu_sub_t& mcpusub);

/* Mach-O CPU Sub-type */
enum class mach_filetype_t : uint32_t {
	None       = 0x00000000U,
	Object     = 0x00000001U,
	Execute    = 0x00000002U,
	FVMLib     = 0x00000003U,
	Core       = 0x00000004U,
	Preload    = 0x00000005U,
	Dylib      = 0x00000006U,
	Dylinker   = 0x00000007U,
	Bundle     = 0x00000008U,
	DylibStub  = 0x00000009U,
	DSym       = 0x0000000AU,
	KEXTBundle = 0x0000000BU,
};
extern const std::array<const enum_pair_t<mach_filetype_t>, 17> mach_filetype_s;
extern std::ostream& operator<<(std::ostream& out, const mach_filetype_t& mftype);


enum class mach_flags_t : uint32_t {
	None                = 0x00000000U,
	NoUndefs            = 0x00000001U,
	IncrLink            = 0x00000002U,
	DyndLink            = 0x00000004U,
	BinDataLoad         = 0x00000008U,
	PreBound            = 0x00000010U,
	SplitSegs           = 0x00000020U,
	LazyInit            = 0x00000040U,
	TwoLevel            = 0x00000080U,
	ForceFlat           = 0x00000100U,
	NoMultiDefs         = 0x00000200U,
	NoFixePreBinding    = 0x00000400U,
	Prebindable         = 0x00000800U,
	AllModsBound        = 0x00001000U,
	SubsectionsViaSyms  = 0x00002000U,
	Canonical           = 0x00004000U,
	WeakDefines         = 0x00008000U,
	BindsToWeak         = 0x00010000U,
	AllowStackExecution = 0x00020000U,
	RootSafe            = 0x00040000U,
	SetUIDSafe          = 0x00080000U,
	NoReexportedDylibs  = 0x00100000U,
	PIE                 = 0x00200000U,
	DeadStrippableDylib = 0x00400000U,
	HasTLVDescriptions  = 0x00800000U,
	NoHeapExecution     = 0x01000000U,
	AppExtensionSafe    = 0x02000000U,
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

	[[nodiscard]]
	bool is_valid() const  noexcept {
		return (_magic1 == 0xFEU && _magic2 == 0xEDU
			 && _magic3 == 0xFAU && _magic4 == 0xCEU);
	}

	void set() noexcept {
		_magic1 = 0xFEU;
		_magic2 = 0xEDU;
		_magic3 = 0xFAU;
		_magic4 = 0xCEU;
	}

	[[nodiscard]]
	std::array<uint8_t, 4> get() const noexcept {
		return { _magic1, _magic2, _magic3, _magic4 };
	}
};


/* 32-Bit Mach-O header */
struct mach32_header_t final {
private:
	mach_magic_t _magic;    /* Should always be 0xFEEDFACE */
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

	constexpr mach32_header_t(
		mach_magic_t magic, mach_cpu_t cpu, mach_cpu_sub_t subcpu,
		uint32_t filetype, uint32_t ncmds, uint32_t cmd_size,
		mach_flags_t flags) noexcept :
		_magic{magic}, _cpu{cpu}, _subcpu{subcpu}, _filetype{filetype},
		_ncmds{ncmds}, _cmd_size{cmd_size}, _flags{flags} { /* NOP */ }


	// void magic()

};



#endif /* __SNS_MACHO_HH__ */

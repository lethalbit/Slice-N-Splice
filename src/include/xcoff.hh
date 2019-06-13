/* xcoff.hh - XCOFF structures and utilities */
#pragma once
#if !defined(__SNS_XCOFF_HH__)
#define __SNS_XCOFF_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <mmap_t.hh>
#include <fd_t.hh>
#include <utility.hh>


struct xcoff_types_32_t final {
	using offset_t = uint32_t;
	using addr_t   = uint32_t;
	using xsize_t  = uint32_t;
	using half_t   = uint16_t;

	constexpr static uint16_t magic = 0x01DF;
};

struct xcoff_types_64_t final {
	using offset_t = uint64_t;
	using addr_t   = uint64_t;
	using xsize_t  = uint64_t;
	using half_t   = uint16_t;

	constexpr static uint16_t magic = 0x01F7;
};

enum class xcoff_hdr_flags_t : uint16_t {
	None        = 0x0000,
	NoRealoc    = 0x0001,
	Exec        = 0x0002,
	NoLineNos   = 0x0004,
	Reserved_1  = 0x0008,
	FDPRProf    = 0x0010,
	FDPROpti    = 0x0020,
	DSASupport  = 0x0040,
	Reserved_2  = 0x0080,
	VarPageSize = 0x0100,
	Reserved_3  = 0x0200,
	Reserved_4  = 0x0400,
	Reserved_5  = 0x0800,
	DynamicLoad = 0x1000,
	SharedObj   = 0x2000,
	LoadOnly    = 0x4000,
	Reserved_6  = 0x8000,
};
template<>
struct EnableBitmask<xcoff_hdr_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<xcoff_hdr_flags_t>, 17> xcoff_hdr_flags_s;
extern std::ostream& operator<<(std::ostream& out, const xcoff_hdr_flags_t& xhdrflag);

enum class xcoff_opthdr_flags_t : uint16_t {
	None = 0x0000,
};
template<>
struct EnableBitmask<xcoff_opthdr_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<xcoff_opthdr_flags_t>, 17> xcoff_opthdr_flags_s;
extern std::ostream& operator<<(std::ostream& out, const xcoff_opthdr_flags_t& xopthdrflag);

template<typename T>
struct xcoff_hdr final {
	using offset_t = typename T::offset_t;
private:
	uint16_t _magic;
	uint16_t _sec_num;
	int32_t _timdat; /* Signed? Unsigned? IBM didn't say */
	offset_t _symtbl;
	uint32_t _sym_count;
	uint16_t _opthdr_offset;
	xcoff_hdr_flags_t _flags;
public:
	constexpr xcoff_hdr() noexcept :
		_magic{}, _sec_num{}, _timdat{}, _symtbl{}, _sym_count{},
		_opthdr_offset{}, _flags{} { /* NOP */ }

	constexpr xcoff_hdr(uint16_t magic, uint16_t sec_num, uint32_t timdat,
			offset_t symtbl, uint32_t sym_count, uint16_t opthdr_offset,
			xcoff_hdr_flags_t flags) noexcept :
		_magic{magic}, _sec_num{sec_num}, _timdat{timdat}, _symtbl{symtbl},
		_sym_count{sym_count}, _opthdr_offset{opthdr_offset}, _flags{flags}
		{ /* NOP */ }

	void magic(const uint16_t magic) noexcept { _magic = magic; }
	uint16_t magic() const noexcept { return _magic; }

	void sec_num(const uint16_t sec_num) noexcept { _sec_num = sec_num; }
	uint16_t sec_num() const noexcept { return _sec_num; }

	void timdat(const int32_t timdat) noexcept { _timdat = timdat; }
	int32_t timdat() const noexcept { return _timdat; }

	void symtbl(const offset_t symtbl) noexcept { _symtbl = symtbl; }
	offset_t symtbl() const noexcept { return _symtbl; }

	void sym_count(const uint32_t sym_count) noexcept { _sym_count = sym_count; }
	uint32_t sym_count() const noexcept { return _sym_count; }

	void opthdr_offset(const uint16_t opthdr_offset) noexcept { _opthdr_offset = opthdr_offset; }
	uint16_t opthdr_offset() const noexcept { return _opthdr_offset; }

	void flags(const xcoff_hdr_flags_t flags) noexcept { _flags = flags; }
	xcoff_hdr_flags_t flags() const noexcept { return _flags; }
};
using xcoff32_hdr_t = xcoff_hdr<xcoff_types_32_t>;
using xcoff64_hdr_t = xcoff_hdr<xcoff_types_64_t>;


template<typename T>
struct xcoff_opthdr_t final {
	using offset_t = typename T::offset_t;
	using addr_t   = typename T::addr_t;
	using xsize_t  = typename T::xsize_t;
	using half_t   = typename T::half_t;
private:
	xcoff_opthdr_flags_t _flags;
	uint16_t _vstamp;
	/* XCOFF32 ordered */
	enabled_if_sv<T, xcoff_types_32_t, xsize_t> _text_size;
	enabled_if_sv<T, xcoff_types_32_t, xsize_t> _data_size;
	enabled_if_sv<T, xcoff_types_32_t, xsize_t> _bss_size;
	enabled_if_sv<T, xcoff_types_32_t, addr_t> _entry;
	enabled_if_sv<T, xcoff_types_32_t, addr_t> _text_start;
	enabled_if_sv<T, xcoff_types_32_t, addr_t> _data_start;
	// enabled_if_sv<T, xcoff_types_32_t, half_t> _toc_anchor;
	// enabled_if_sv<T, xcoff_types_32_t, half_t> _entry_sn;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _text_sn;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _data_sn;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _toc_sn;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _loader_sn;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _bss_sn;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _text_align;
	// enabled_if_sv<T, xcoff_types_32_t, uint16_t> _data_align;
	/* XCOFF64 ordered */

	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _entry_sn;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _text_sn;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _data_sn;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _toc_sn;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _loader_sn;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _bss_sn;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _text_align;
	// enabled_if_sv<T, xcoff_types_64_t, uint16_t> _data_align;

public:
	constexpr xcoff_opthdr_t(
		std::enable_if_t<std::is_same_v<T, xcoff_types_32_t>> * = nullptr) noexcept :
		_flags{}, _vstamp{}, _text_size{} { /* NOP */ }

	constexpr xcoff_opthdr_t(
		std::enable_if_t<std::is_same_v<T, xcoff_types_64_t>> * = nullptr) noexcept :
		_flags{}, _vstamp{} { /* NOP */ }
};

using xcoff32_opthdr_t = xcoff_opthdr_t<xcoff_types_32_t>;
using xcoff64_opthdr_t = xcoff_opthdr_t<xcoff_types_64_t>;

#endif /* __SNS_XCOFF_HH__ */
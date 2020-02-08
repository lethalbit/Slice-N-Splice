/* utility.hh - Miscellaneous utilities */
#pragma once
#if !defined(__SNS_UTILITY_HH__)
#define __SNS_UTILITY_HH__

#ifndef __has_cpp_attribute
	#define __has_cpp_attribute(x) 0
#endif

#if __has_cpp_attribute(alignas)
	#define ALIGN(X) [[alignas(X)]]
#else
	#define ALIGN(X) __attribute__ ((aligned (X)))
#endif

#include <algorithm>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


#include <sys/stat.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>
#include <utility>


/* SFINAE helpers */
template<typename T1, typename T2, typename T3> using enabled_if_sv =
	typename std::enable_if_t<std::is_same_v<T1, T2>, T3>;

template<typename T1> using enabled_if_enum =
	std::enable_if_t<std::is_enum_v<T1>, void*>;

template<typename T1> using enabled_ifn_enum =
	std::enable_if_t<!std::is_enum_v<T1>, void*>;

/* Enum bitmasks */

template<typename F>
struct EnableBitmask{
	static constexpr bool enabled = false;
};

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F>
operator|(F lh, F rh) {
	using utype = typename std::underlying_type_t<F>;
	return static_cast<F>(
		static_cast<utype>(lh) | static_cast<utype>(rh)
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F1>::enabled && std::is_integral_v<F2>,F1>
operator|(F1 lh, F2 rh) {
	using utype = typename std::underlying_type_t<F1>;
	return static_cast<F1>(
			static_cast<utype>(lh) | rh
		);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F2>::enabled && std::is_integral_v<F1>,F2>
operator|(F1 lh, F2 rh) {
	using utype = typename std::underlying_type_t<F2>;
	return static_cast<F2>(
			lh | static_cast<utype>(rh)
		);
}

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F>
operator&(F lh, F rh) {
	using utype = typename std::underlying_type_t<F>;
	return static_cast<F>(
		static_cast<utype>(lh) & static_cast<utype>(rh)
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F1>::enabled && std::is_integral_v<F2>,F1>
operator&(F1 lh, F2 rh) {
	using utype = typename std::underlying_type_t<F1>;
	return static_cast<F1>(
			static_cast<utype>(lh) & rh
		);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F2>::enabled && std::is_integral_v<F1>,F2>
operator&(F1 lh, F2 rh) {
	using utype = typename std::underlying_type_t<F2>;
	return static_cast<F2>(
			lh & static_cast<utype>(rh)
		);
}

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F>
operator~(F en) {
	using utype = typename std::underlying_type_t<F>;
	return static_cast<F>(
		~static_cast<utype>(en)
	);
}

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F>
operator^(F lh, F rh) {
	using utype = typename std::underlying_type_t<F>;
	return static_cast<F>(
		static_cast<utype>(lh) ^ static_cast<utype>(rh)
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F1>::enabled && std::is_integral_v<F2>,F1>
operator^(F1 lh, F2 rh) {
	using utype = typename std::underlying_type_t<F1>;
	return static_cast<F1>(
			static_cast<utype>(lh) ^ rh
		);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F2>::enabled && std::is_integral_v<F1>,F2>
operator^(F1 lh, F2 rh) {
	using utype = typename std::underlying_type_t<F2>;
	return static_cast<F2>(
			lh ^ static_cast<utype>(rh)
		);
}

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F&>
operator|=(F& lh, F rh) {
	using utype = typename std::underlying_type_t<F>;
	return lh = static_cast<F>(
		static_cast<utype>(lh) | static_cast<utype>(rh)
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F1>::enabled && std::is_integral_v<F2>, F1&>
operator|=(F1& lh, F2 rh) {
	using utype = typename std::underlying_type_t<F1>;
	return lh = static_cast<F1>(
		static_cast<utype>(lh) | rh
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F2>::enabled && std::is_integral_v<F1>, F1&>
operator|=(F1& lh, F2 rh) {
	using utype = typename std::underlying_type_t<F2>;
	return lh = (
		lh | static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F&>
operator&=(F& lh, F rh) {
	using utype = typename std::underlying_type_t<F>;
	return lh = static_cast<F>(
		static_cast<utype>(lh) & static_cast<utype>(rh)
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F1>::enabled && std::is_integral_v<F2>, F1&>
operator&=(F1& lh, F2 rh) {
	using utype = typename std::underlying_type_t<F1>;
	return lh = static_cast<F1>(
		static_cast<utype>(lh) & rh
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F2>::enabled && std::is_integral_v<F1>, F1&>
operator&=(F1& lh, F2 rh) {
	using utype = typename std::underlying_type_t<F2>;
	return lh = (
		lh & static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if_t<EnableBitmask<F>::enabled,F&>
operator^=(F& lh, F rh) {
	using utype = typename std::underlying_type_t<F>;
	return lh = static_cast<F>(
		static_cast<utype>(lh) ^ static_cast<utype>(rh)
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F1>::enabled && std::is_integral_v<F2>, F1&>
operator^=(F1& lh, F2 rh) {
	using utype = typename std::underlying_type_t<F1>;
	return lh = static_cast<F1>(
		static_cast<utype>(lh) ^ rh
	);
}

template<typename F1, typename F2>
typename std::enable_if_t<EnableBitmask<F2>::enabled && std::is_integral_v<F1>, F1&>
operator^=(F1& lh, F2 rh) {
	using utype = typename std::underlying_type_t<F2>;
	return lh = (
		lh ^ static_cast<utype>(rh)
	);
}

/* IEC Units*/
constexpr uint64_t operator ""_KiB(const unsigned long long value) noexcept { return uint64_t(value) * 1024; }
constexpr uint64_t operator ""_MiB(const unsigned long long value) noexcept { return uint64_t(value) * 1048576; }
constexpr uint64_t operator ""_GiB(const unsigned long long value) noexcept { return uint64_t(value) * 1073741824; }

/* SI Units */
constexpr uint64_t operator ""_KB(const unsigned long long value) noexcept { return uint64_t(value) * 1000; }
constexpr uint64_t operator ""_MB(const unsigned long long value) noexcept { return uint64_t(value) * 1000000; }
constexpr uint64_t operator ""_GB(const unsigned long long value) noexcept { return uint64_t(value) * 1000000000; }

/* enum <-> string mapping */

template<class T>
struct enum_pair_t final {
	using value_type = T;
private:
	T _value;
	const char* _name;
public:
	constexpr enum_pair_t() noexcept :
		_value{}, _name{} { /* NOP */ }

	constexpr enum_pair_t(T value, const char* name) noexcept :
		_value{value}, _name{name} { /* NOP */ }

	void value(const T value) noexcept { _value = value; }
	[[nodiscard]]
	T value() const noexcept { return _value; }

	void name(const char* name) noexcept { _name = name; }
	[[nodiscard]]
	const char* name() const noexcept { return _name; }
};

template<class Map, class Val>
std::string enum_name(Map m, Val v) noexcept {
	auto pos = std::find_if(
		std::begin(m),
		std::end(m),
		[&v](const typename Map::value_type& t){
			return (t.value() == v);
		});
	return (pos != std::end(m)) ?
		pos->name() :
		"UNKNOWN";
}

template<class Map>
typename Map::value_type::value_type enum_value(Map m, const std::string& n) noexcept {
	auto pos = std::find_if(
		std::begin(m),
		std::end(m),
		[&n](const typename Map::value_type& t){
			return (t.name() == n);
		});

	return (pos != std::end(m)) ?
		pos->value() :
		static_cast<typename Map::value_type::value_type>(0);
}



uint16_t _sns_bswap16(const uint16_t x) noexcept;
uint32_t _sns_bswap32(const uint32_t x) noexcept;
uint64_t _sns_bswap64(const uint64_t x) noexcept;


/* Extract a collection of flags set in a field */
/* This is kind of expensive run-time wise, being at leas O(n+1) but *shrug* */
template<typename T, typename A>
std::enable_if_t<std::is_enum_v<T>, std::vector<T>>
extract_flags(T flags, A& enum_table) {
	std::vector<T> _found_flags;
	using ut = typename std::underlying_type_t<T>;
	for (auto flag : enum_table) {
		if((flags & flag.value()) == flag.value() && static_cast<ut>(flag.value()) != 0)
			_found_flags.emplace_back(flag.value());
	}

	if(_found_flags.size() == 0)
		_found_flags.emplace_back(T{});

	return _found_flags;
}

template<typename T, typename A>
std::enable_if_t<std::is_enum_v<T>, std::vector<enum_pair_t<T>>>
extract_flag_pairs(T flags, A& enum_table) {
	std::vector<enum_pair_t<T>> _found_flags;
	using ut = typename std::underlying_type_t<T>;
	for (auto flag : enum_table) {
		if((flags & flag.value()) == flag.value() && static_cast<ut>(flag.value()) != 0)
			_found_flags.emplace_back(flag);
	}

	if(_found_flags.size() == 0)
		_found_flags.emplace_back(enum_table[0]);

	return _found_flags;
}


#endif /* __SNS_UTILITY_HH__ */

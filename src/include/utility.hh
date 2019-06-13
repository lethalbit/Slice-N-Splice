/* utility.hh - Miscellaneous utilities */
#pragma once
#if !defined(__SNS_UTILITY_HH__)
#define __SNS_UTILITY_HH__



#ifndef __has_cpp_attribute
	#define __has_cpp_attribute(x) 0
#endif

#if __has_cpp_attribute(nodiscard)
	#define NODSC [[nodiscard]]
#else
	#define NODSC
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

#include <sys/stat.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>
#include <utility>


/* Some basic logging */
#if !defined(NDEBUG)
#include <cstdio>
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...)
#endif

#if !defined(NDEBUG)


#else
// #define assert(expr, msg) ((void)0)
#endif

/**/
template<typename T1, typename T2, typename T3> using enabled_if_sv =
	typename std::enable_if_t<std::is_same_v<T1, T2>, T3>;


template<typename T>
struct fixed_iterator_t final {
private:
	T* const _data;
	size_t _index;
	const size_t _len;

	constexpr fixed_iterator_t(T* const data,
			const size_t index, const size_t len) noexcept :
		_data{data}, _index{index}, _len{len} { /* NOP */ }

public:
	constexpr fixed_iterator_t(T* const data, const size_t len) noexcept :
		_data{data}, _index{0}, _len{len} { /* NOP */ }

	fixed_iterator_t(const fixed_iterator_t& itr) noexcept :
		_data{itr._data}, _index{itr._index}, _len{itr._len} { /* NOP */ }

	T& operator*() const noexcept { return _data[_index]; }
	T* operator->() const noexcept { return _data[_index]; }

	fixed_iterator_t& operator++() noexcept { if(_index < _len)  ++_index; return *this; }
	fixed_iterator_t operator++(int) noexcept {
		if(_index < _len)
			return {_data, _index++, _len};
		return *this;
	}

	fixed_iterator_t& operator--() noexcept { if(_index > 0) --_index; return *this; }
	fixed_iterator_t operator--(int) noexcept {
		if(_index > 0)
			return {_data, _index--, _len};
		return *this;
	}

	T& operator[](const size_t idx) const noexcept { return *(*this + idx); }

	fixed_iterator_t& operator+=(const size_t num) noexcept {
		if(((_index + num) < _len) && ((_index + num) >= _index))
			_index += num;
		else
			_index = _len;

		return *this;
	}

	fixed_iterator_t operator+(const size_t num) const noexcept {
		return fixed_iterator_t(*this) += num;
	}

	fixed_iterator_t& operator-=(const size_t num) noexcept {
		if(num < _index)
			_index -= num;
		else
			_index = 0;

		return *this;
	}

	fixed_iterator_t operator-(const size_t num) const noexcept {
		return fixed_iterator_t(*this) -= num;
	}

	bool operator==(const fixed_iterator_t& itr) const noexcept {
		return _data == itr._data   &&
		       _index == itr._index &&
		       _len == itr._len;
	}

	bool operator !=(const fixed_iterator_t& itr) const noexcept { return !(*this == itr); }

};


template<typename T>
struct set_t final {
private:
	T* _data;
	size_t _size;
public:
	constexpr set_t() noexcept :
		_data{nullptr}, _size{0} { /* NOP */ }

	set_t(T* data) noexcept :
		_data{data}, _size{0} { /* NOP */ }

	set_t(T* data, size_t size) noexcept :
		_data{data}, _size{size} { }

	void data(const T* data) noexcept { _data = data; }
	T* data() const noexcept { return _data; }

	void size(const size_t size) noexcept { _size = size; }
	size_t size() const noexcept { return _size; }


	size_t length() const noexcept { return _size; }
	size_t count() const noexcept { return _size; }



	T& operator[](const size_t idx) {
		if(idx <= _size)
			return _data[idx];
		else
			return _data[0];
	}

	fixed_iterator_t<T> begin() noexcept { return fixed_iterator_t<T>(_data, _size); }
	fixed_iterator_t<T> end() noexcept { return (begin() + _size); }

	fixed_iterator_t<const T> begin() const noexcept { return fixed_iterator_t<T>(_data, _size); }
	fixed_iterator_t<const T> end() const noexcept { return (begin() + _size); }


	void swap(set_t& set) noexcept {
		std::swap(_data, set._data);
		std::swap(_size, set._size);
	}


	// set_t(const set_t&) = delete;
	// set_t& operator=(const set_t&) = delete;

};

/* Enum bitmasks */

template<typename F>
struct EnableBitmask{
	static constexpr bool enabled = false;
};

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled,F>::type
operator|(F lh, F rh) {
	typedef typename std::underlying_type<F>::type utype;
	return static_cast<F>(
		static_cast<utype>(lh) | static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled,F>::type
operator&(F lh, F rh) {
	typedef typename std::underlying_type<F>::type utype;
	return static_cast<F>(
		static_cast<utype>(lh) & static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled, F>::type
operator~(F en) {
	typedef typename std::underlying_type<F>::type utype;
	return static_cast<F>(
		~static_cast<utype>(en)
	);
}

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled,F>::type
operator^(F lh, F rh) {
	typedef typename std::underlying_type<F>::type utype;
	return static_cast<F>(
		static_cast<utype>(lh) ^ static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled,F&>::type
operator|=(F& lh, F rh) {
	typedef typename std::underlying_type<F>::type utype;
	return lh = static_cast<F>(
		static_cast<utype>(lh) | static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled,F&>::type
operator&=(F& lh, F rh) {
	typedef typename std::underlying_type<F>::type utype;
	return lh = static_cast<F>(
		static_cast<utype>(lh) & static_cast<utype>(rh)
	);
}

template<typename F>
typename std::enable_if<EnableBitmask<F>::enabled,F&>::type
operator^=(F& lh, F rh) {
	typedef typename std::underlying_type<F>::type utype;
	return lh = static_cast<F>(
		static_cast<utype>(lh) ^ static_cast<utype>(rh)
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
	T _value;
	const char* _name;

	void value(const T value) noexcept { _value = value; }
	T value() const noexcept { return _value; }

	void name(const char* name) noexcept { _name = name; }
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

constexpr uint16_t _sns_bswap16(const uint16_t x) noexcept;
constexpr uint32_t _sns_bswap32(const uint32_t x) noexcept;
constexpr uint64_t _sns_bswap64(const uint64_t x) noexcept;



#endif /* __SNS_UTILITY_HH__ */

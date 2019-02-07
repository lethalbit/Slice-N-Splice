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
#include <sys/mman.h>
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

template<class Child>
class Singleton {
public:
	static Child* GetInstance(void);
private:
	static std::unique_ptr<Child> _Instance;
};

template <class Child>
std::unique_ptr<Child> Singleton<Child>::_Instance = nullptr;

template<class Child>
Child* Singleton<Child>::GetInstance(void) {
	if(_Instance != nullptr)
		return _Instance.get();
	auto ut = std::make_unique<Child>();
	_Instance = std::move(ut);
	return _Instance.get();
}


struct mmap_t final{
private:
	int32_t fd;
	void *addr;
	off_t len;

	template<typename T> T &index(const off_t idx) const {
		if (idx < len)
			return *reinterpret_cast<T *const>(long(addr) + idx);
		throw std::out_of_range("index out of range in mmap_t");
	}

public:
	constexpr mmap_t() noexcept : fd(-1), addr(nullptr), len(0) { }
	mmap_t(const int32_t file, const off_t length, const int32_t prot) noexcept : fd(file),
			addr(mmap(nullptr, length, prot, MAP_SHARED, fd, 0)), len(length)
		{ if (addr == MAP_FAILED) addr = nullptr; }
	mmap_t(mmap_t &&file) noexcept : mmap_t() { swap(file); }
	~mmap_t() noexcept { if (fd != -1) close(fd); munmap(addr, len); }
	void operator =(mmap_t &&file) noexcept { swap(file); }

	NODSC bool valid() const noexcept  { return fd != -1 && addr; }
	void swap(mmap_t &file) noexcept {
		std::swap(fd, file.fd);
		std::swap(addr, file.addr);
		std::swap(len, file.len);
	}

	template<typename T> T *address() noexcept { return static_cast<T *>(addr); }
	template<typename T> const T *address() const noexcept { return static_cast<T *const>(addr); }
	NODSC off_t length() const noexcept  { return len; }
	template<typename T> T &operator [](const off_t idx) { return index<T>(idx); }
	template<typename T> const T &operator [](const off_t idx) const { return index<const T>(idx); }
	template<typename T> T &at(const off_t idx) { return index<T>(idx); }
	template<typename T> const T &at(const off_t idx) const { return index<const T>(idx); }

	bool operator ==(const mmap_t &b) const noexcept { return fd == b.fd && addr == b.addr && len == b.len; }
	bool operator !=(const mmap_t &b) const noexcept { return !(*this == b); }
};

struct fd_t final {
private:
	int32_t fd;
	mutable bool eof;

	template<typename T, typename U = T> inline T exchange(T &obj, U &&newVal) {
		T oldVal = std::move(obj);
		obj = std::forward<U>(newVal);
		return oldVal;
	}


public:
	constexpr fd_t() noexcept : fd(-1), eof(false) { }
	constexpr fd_t(const int32_t file) noexcept : fd(file), eof(false) { }
	fd_t(const char *const file, int flags, mode_t mode = 0) noexcept : fd(::open(file, flags, mode)), eof(false) { }
	fd_t(fd_t &&file) noexcept : fd_t() { swap(file); }
	~fd_t() noexcept { if (fd != -1) close(fd); }
	void operator =(fd_t &&file) noexcept { swap(file); }

	NODSC operator int32_t() const noexcept { return fd; }
	NODSC bool operator ==(const int32_t desc) const noexcept { return fd == desc; }
	NODSC bool valid() const noexcept { return fd != -1; }
	NODSC bool isEOF() const noexcept { return eof; }
	void swap(fd_t &file) noexcept {
		std::swap(fd, file.fd);
		std::swap(eof, file.eof);
	}

	template<typename T> bool read(T &value) const noexcept
		{ return read(&value, sizeof(T)); }
	template<typename T> bool read(const std::unique_ptr<T> &value, const size_t valueLen) const noexcept
		{ return read(value.get(), valueLen); }

	template<typename T, size_t N> bool read(std::array<T, N> &value, const size_t count_ = N) const noexcept {
		const size_t count = count < N ? count : N;
		bool result = true;
		for (size_t i = 0; i < count; ++i)
			result &= read(value[i]);
		return result;
	}


	template<typename T> bool write(T &value) const noexcept
		{ return write(&value, sizeof(T)); }
	template<typename T> bool write(const std::unique_ptr<T> &value, const size_t valueLen) const noexcept
		{ return write(value.get(), valueLen); }

	template<typename T, size_t N> bool write(const std::array<T, N> &value, const size_t count_ = N) const noexcept {
		const size_t count = count_ < N ? count_ : N;
		bool result = true;
		for (size_t i = 0; i < count; ++i)
			result &= write(value[i]);
		return result;
	}

	NODSC bool read(void *const value, const size_t valueLen) const noexcept {
		size_t actualLen;
		return read(value, valueLen, actualLen);
	}

	NODSC bool read(void *const value, const size_t valueLen, size_t &actualLen) const noexcept {
		actualLen = 0;
		if (eof)
			return 0;
		const ssize_t result = ::read(fd, value, valueLen);
		if (result == 0 && valueLen != 0)
			eof = true;
		else if (result > 0)
			actualLen = size_t(result);
		return actualLen == valueLen;
	}
	ssize_t write(const void *const bufferPtr, const size_t len) const noexcept { return ::write(fd, bufferPtr, len); }
	NODSC off_t seek(off_t offset, int32_t whence) const noexcept { return ::lseek(fd, offset, whence); }
	NODSC off_t tell() const noexcept { return seek(0, SEEK_CUR); }

	NODSC off_t length() const noexcept {
		struct stat fileStat;
		if (!fstat(fd, &fileStat))
			return fileStat.st_size;
		return 0;
	}

	NODSC mmap_t map(const int32_t prot) noexcept
		{ return map(prot, length()); }

	NODSC mmap_t map(const int32_t prot, const off_t len) noexcept {
		// If we don't represent a valid file desriptor, don't return a valid mmap_t,
		// but rather invoke it's default constructor.
		if (!valid())
			return {};
		int32_t file = exchange(fd, -1);
		return {file, len, prot};
	}

	fd_t(const fd_t &) = delete;
	fd_t &operator =(const fd_t &) = delete;
};

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

#endif /* __SNS_UTILITY_HH__ */

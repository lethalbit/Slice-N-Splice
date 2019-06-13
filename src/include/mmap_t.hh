/* mmap_t.hh */
#pragma once
#if !defined(__SNS_MMAP_T_HH__)
#define __SNS_MMAP_T_HH__
#include <sys/mman.h>
#include <utility>

#include <utility.hh>

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

#endif /* __SNS_MMAP_T_HH__ */

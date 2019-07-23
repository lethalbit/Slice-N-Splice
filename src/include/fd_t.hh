/* fd_t.hh - RAII File wrapper */
#pragma once
#if !defined(__SNS_FD_T_HH__)
#define __SNS_FD_T_HH__
#include <utility.hh>

#include <mmap_t.hh>

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
		const size_t count = count_ < N ? count_ : N;
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
#endif /* __SNS_FD_T_HH__ */

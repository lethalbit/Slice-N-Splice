/* zlib.hh - zlib RAII wrapper and streaming compression API */
#pragma once
#if !defined(__SNS_ZLIB_HH__)
#define __SNS_ZLIB_HH__

#include <zlib.h>
#include <cstdint>

#include <utility.hh>
#include <vector>

enum class zlib_mode : uint8_t {
	inflate,
	deflate,
};

struct zlib_t final {
private:
	constexpr static const uint64_t chunk_size{128_KiB};
	int flush;
	z_stream stream;
	zlib_mode mode;
	uint8_t input_buffer[chunk_size];
	uint8_t output_buffer[chunk_size];
	bool eos;
	size_t have;

public:
	constexpr zlib_t() noexcept :
		flush{}, stream{}, mode{zlib_mode::deflate}, input_buffer{},
		output_buffer{}, eos{}, have{} {
		/* I know, I know, shhhh */
		::deflateInit(&stream, Z_DEFAULT_COMPRESSION);
	}

	zlib_t(zlib_mode mode) noexcept :
		flush{}, stream{}, mode{mode}, input_buffer{},
		output_buffer{}, eos{}, have{}  {

		if(mode == zlib_mode::inflate)
			::inflateInit(&stream);
		else
			::deflateInit(&stream, Z_DEFAULT_COMPRESSION);
	}

	~zlib_t() noexcept {
		if(mode == zlib_mode::inflate)
			::inflateEnd(&stream);
		else
			::deflateEnd(&stream);
	}

	template<typename T>
	size_t transform(T& value, void* output) noexcept {
		return transform(&value, sizeof(T), output);
	}

	template<typename T, size_t N>
	size_t transform(std::array<T, N>& value, void* output) noexcept {
		return transform(value.data(), N, output);
	}

	size_t transform(void* const value, const size_t len, void* output) noexcept;
};

#endif /* __SNS_ZLIB_HH__ */

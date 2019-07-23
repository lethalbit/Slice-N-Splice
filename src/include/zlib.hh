/* zlib.hh - zlib RAII wrapper and streaming compression API */
#pragma once
#if !defined(__SNS_ZLIB_HH__)
#define __SNS_ZLIB_HH__

#include <zlib.h>
#include <cstdint>

#include <utility.hh>
#include <vector>


struct zlib_t final {
private:
	constexpr static const uint64_t chunk_size{128_KiB};
	int flush;
	z_stream stream;
	uint8_t input_buffer[chunk_size];
	uint8_t output_buffer[chunk_size];
	bool eos;
	size_t have;

public:
	constexpr zlib_t() noexcept :
		flush{}, stream{}, input_buffer{}, output_buffer{}, eos{}, have{} {
	}

	// template<typename T>
	// size_t transform(T& value, void* output) noexcept {
	// 	return transform(&value, sizeof(T), output);
	// }

	// template<typename T, size_t N>
	// size_t transform(std::array<T, N>& value, void* output) noexcept {
	// 	return transform(value.data(), N, output);
	// }

	// size_t transform(void* const value, const size_t len, void* output) noexcept;
};

#endif /* __SNS_ZLIB_HH__ */

/* zlib.hh - zlib RAII wrapper and streaming compression API */
#pragma once
#if !defined(__SNS_ZLIB_HH__)
#define __SNS_ZLIB_HH__

#include <zlib.h>
#include <cstdint>

#include <utility.hh>
#include <vector>


struct zlib_t final {
public:
	enum class mode_t : uint8_t {
		Inflate,
		Deflate
	};

	struct zlib_ctx_t final {
	private:
		constexpr static const uint64_t chunk_size{128_KiB};
		int _flush;
		zlib_t::mode_t _mode;
		z_stream _stream;
		uint8_t _input_buffer[chunk_size];
		uint8_t _output_buffer[chunk_size];
		bool _eos;
		size_t _have;
	public:
		zlib_ctx_t(zlib_t::mode_t mode) noexcept :
			_flush{}, _mode{mode}, _stream{}, _input_buffer{}, _output_buffer{},
			_eos{}, _have{} {

			if(mode == zlib_t::mode_t::Inflate)
				_eos = (::inflateInit(&_stream) != Z_OK);
			else if(mode == zlib_t::mode_t::Deflate)
				_eos = (::deflateInit(&_stream, 9) != Z_OK);
		}

		zlib_t::mode_t mode() const noexcept { return _mode; }

		template<typename T>
		size_t transform(T& value, void* output) noexcept {
			return transform(&value, sizeof(T), output);
		}

		template<typename T, size_t N>
		size_t transform(std::array<T, N>& value, void* output) noexcept {
			return transform(value.data(), N, output);
		}

		size_t transform(void* const value, const size_t len, void* output) noexcept {
			return 0;
		}
	};

private:
	zlib_ctx_t _zlib_deflate;
	zlib_ctx_t _zlib_inflate;
public:
	zlib_t() noexcept :
		_zlib_deflate{zlib_t::mode_t::Deflate},
		_zlib_inflate{zlib_t::mode_t::Inflate}
	{ /* NOP */ }


	template<typename T>
	T inflate(uint8_t* buff, size_t size) {
		T _tmp;
		_zlib_inflate.transform(buff, size, &_tmp);
		return std::move(_tmp);
	}

	template<typename T, size_t N>
	T inflate(uint8_t* buff) {
		T _tmp;
		_zlib_inflate.transform(buff, N, &_tmp);
		return std::move(_tmp);
	}

};

#endif /* __SNS_ZLIB_HH__ */

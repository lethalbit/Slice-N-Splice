/* zlib.cc - zlib RAII wrapper and streaming compression API */

#include <zlib.hh>

#include <cstring>

#include <cstdio>

size_t zlib_t::transform(void* const value, const size_t len, void* output) noexcept {
	size_t transformed = 0;
	size_t to_chunk = 0;
	off_t chunk_offset = 0;
	int err = 0;

	/* Setup the things that shouldn't be moving */


	/* if we can't fill the chunk, we want just the length */
	to_chunk = (chunk_size > len) ? len : chunk_size;
	std::memset(input_buffer, 0, chunk_size);

	if(mode == zlib_mode::deflate) {
		do {
			std::memcpy(input_buffer, static_cast<uint8_t*>((value + chunk_offset)), to_chunk);
			stream.avail_in = to_chunk;
			printf("to_chunk: %d, chunk_offset: %d\n", to_chunk, chunk_offset);

			flush = (transformed == len) ? Z_FINISH : Z_NO_FLUSH;
			stream.next_in = input_buffer;
			printf("flush: %d\n", flush);

			do {
				stream.avail_out = chunk_size;
				stream.next_out = output_buffer;

				if((err = ::deflate(&stream, flush)) != Z_STREAM_ERROR) {
					have = chunk_size - stream.avail_out;
					printf("have: %d avail_out: %d\n", have, stream.avail_out);
					/* Copy from internal output buffer to external */
					std::memcpy(output, output_buffer, have);
				} else {
					printf("stream error: %d\n", err);
				}

			} while(stream.avail_out == 0);
			chunk_offset += to_chunk;
			transformed += to_chunk;
		} while(flush != Z_FINISH);
	} else {
		/* Mode is inflate */
	}

	return transformed;
}

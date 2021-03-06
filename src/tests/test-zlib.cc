#include <iostream>
#include <cstdio>

#include <catch2/catch.hpp>

#include <zlib.hh>
#include <elf.hh>
#include <utility.hh>

/* Lovingly compressed with
	`python -c 'import sys,zlib; sys.stdout.buffer.write(zlib.compress(sys.stdin.read().encode("utf-8")))'`
*/
static uint8_t zlib_compressed[] = {
	0x78, 0x9c, 0x7d, 0x53, 0x31, 0x92, 0xdb, 0x30, 0x0c, 0xec, 0xf5, 0x0a,
	0x74, 0x6e, 0x74, 0x9e, 0xdc, 0x13, 0xd2, 0x5d, 0x9a, 0xa4, 0x70, 0x95,
	0x12, 0x22, 0x21, 0x8b, 0x67, 0x8a, 0xf0, 0x90, 0x90, 0x1d, 0xfd, 0x3e,
	0x4b, 0x4a, 0x76, 0x72, 0xc9, 0x24, 0x95, 0x04, 0x12, 0x58, 0x60, 0xb1,
	0xcb, 0xb7, 0x60, 0x7c, 0x0e, 0x74, 0x92, 0xf4, 0xae, 0xcb, 0x99, 0x27,
	0xce, 0x4c, 0xa1, 0x50, 0x96, 0x33, 0x67, 0x2f, 0xbe, 0xe3, 0x42, 0x4c,
	0x45, 0x5f, 0x1c, 0xc7, 0x28, 0x9e, 0xee, 0xc2, 0x17, 0x3a, 0x87, 0x1c,
	0x29, 0x24, 0xb2, 0x49, 0xc8, 0x45, 0x2e, 0xe5, 0xd8, 0x51, 0xf7, 0x39,
	0xda, 0x04, 0x84, 0x09, 0xa7, 0xb9, 0x7e, 0x75, 0x31, 0x9a, 0x02, 0xe2,
	0xe2, 0x26, 0xd5, 0x48, 0x05, 0xc9, 0x53, 0x45, 0x8b, 0x77, 0x5e, 0x0b,
	0x0d, 0x22, 0xe9, 0x81, 0x51, 0x78, 0xde, 0x81, 0x08, 0x09, 0xb3, 0xf4,
	0xdd, 0xa8, 0x99, 0xc6, 0x90, 0x8b, 0xd1, 0x2a, 0x9c, 0x7b, 0x2a, 0xe2,
	0x34, 0xf9, 0x16, 0x10, 0xe3, 0xc7, 0xa6, 0x90, 0xf7, 0xf0, 0x05, 0x01,
	0xdb, 0xa1, 0xd4, 0xb3, 0xb2, 0xa7, 0x7f, 0x41, 0xa7, 0x9b, 0x50, 0x92,
	0x9b, 0x64, 0xd2, 0xe4, 0xd0, 0x02, 0xed, 0xba, 0x09, 0x91, 0x57, 0x00,
	0xac, 0xc8, 0x4d, 0x67, 0x62, 0x67, 0xe1, 0x26, 0x47, 0x3a, 0x4d, 0x72,
	0x78, 0x0e, 0xb6, 0xcf, 0x34, 0xb0, 0xbb, 0x90, 0x8e, 0xbf, 0x38, 0x66,
	0xd5, 0xb9, 0xf5, 0xce, 0xc2, 0x1e, 0xd5, 0x1d, 0xd3, 0xa0, 0x7a, 0xa1,
	0x61, 0x25, 0xe0, 0x16, 0x89, 0x63, 0x03, 0xfa, 0x9d, 0x63, 0x61, 0xfb,
	0x2f, 0x5c, 0xdf, 0xed, 0x58, 0x0d, 0xa9, 0x96, 0x69, 0xaa, 0xe3, 0xe8,
	0x2c, 0x16, 0x66, 0xc1, 0x2c, 0x56, 0xb5, 0x60, 0x60, 0x66, 0xef, 0xb4,
	0x92, 0xa9, 0x5c, 0x29, 0xb6, 0x6c, 0x1f, 0xc6, 0x31, 0xb8, 0x25, 0x5a,
	0xdf, 0xa9, 0x55, 0x6a, 0x1f, 0x6b, 0x9c, 0xce, 0xc1, 0x6d, 0x23, 0xde,
	0x83, 0x4d, 0xed, 0xa4, 0x22, 0x78, 0x29, 0xe1, 0x5c, 0x57, 0xef, 0x83,
	0x63, 0xab, 0x44, 0x1a, 0x26, 0xca, 0xee, 0x21, 0x46, 0x5c, 0x3b, 0x0c,
	0x55, 0x84, 0x56, 0x5d, 0x32, 0xb2, 0x4c, 0xa0, 0xbb, 0x33, 0x7a, 0x5f,
	0x20, 0x06, 0xb8, 0x3e, 0x26, 0x0e, 0x56, 0x45, 0x3f, 0x2d, 0xb0, 0xcf,
	0x10, 0xd7, 0xbe, 0xca, 0x8b, 0x1d, 0x96, 0x99, 0xb3, 0xb5, 0x2d, 0xd5,
	0x19, 0xac, 0xd1, 0x35, 0xbd, 0x7e, 0x60, 0x8e, 0xb2, 0xef, 0xba, 0x1c,
	0x3c, 0x5a, 0x8d, 0x21, 0x05, 0x34, 0x58, 0xa1, 0x75, 0x13, 0x15, 0x92,
	0x55, 0x2b, 0xbc, 0xfd, 0xed, 0xc9, 0xae, 0x72, 0xd8, 0x77, 0x59, 0x01,
	0x5f, 0x3f, 0xb5, 0xd4, 0xa7, 0x5c, 0x99, 0xd3, 0x05, 0x63, 0x95, 0xee,
	0x8a, 0x0e, 0xf0, 0x69, 0x75, 0x1f, 0x8f, 0x26, 0xd5, 0x2e, 0x24, 0x3f,
	0x78, 0x6e, 0xd3, 0x22, 0xd1, 0xab, 0x94, 0x74, 0xb0, 0x6a, 0x88, 0x19,
	0x48, 0x9b, 0x51, 0xe0, 0x08, 0x1a, 0x73, 0x90, 0xe4, 0x61, 0xe5, 0xaf,
	0x6a, 0x04, 0xe3, 0x24, 0x18, 0x47, 0xe8, 0x0a, 0x69, 0x35, 0xd5, 0xe2,
	0x6f, 0x23, 0x16, 0xb8, 0x40, 0xe8, 0x1e, 0x17, 0xad, 0xe7, 0xb6, 0xf5,
	0x09, 0x6c, 0xfb, 0xba, 0x2d, 0x72, 0xdc, 0x80, 0x79, 0xdd, 0x54, 0xda,
	0x36, 0x32, 0x48, 0xd3, 0x77, 0x89, 0x31, 0x88, 0x3f, 0x76, 0x7f, 0xbe,
	0xb3, 0xcd, 0x29, 0xfd, 0x43, 0xa2, 0x91, 0xe1, 0xd5, 0x18, 0x2e, 0x82,
	0xfd, 0xa2, 0x38, 0xb1, 0x2d, 0x99, 0x63, 0x5f, 0xdb, 0x65, 0x34, 0xfe,
	0x97, 0x99, 0xfa, 0xa7, 0x2c, 0x9b, 0x29, 0x8f, 0x8d, 0xea, 0x1d, 0x4e,
	0x1c, 0x96, 0x10, 0xf7, 0x8b, 0x3b, 0x5e, 0x30, 0x31, 0xde, 0x26, 0x56,
	0xfd, 0x70, 0x6c, 0xf7, 0x13, 0x99, 0x79, 0x66, 0x60
};
static const size_t zlib_compressed_size = 525;

static char zlib_uncompressed[] = {
	"Hitagi Senjougahara is regarded\n"
	"as a so-called weak girl in the class.\n"
	" \n"
	"Although throughout high school she has always been in the same class as me,\n"
	"for first year, second year and third year - that's this year, I have never once seen\n"
	"her do anything active. She's always in the back of the classroom and reading\n"
	"a book by herself. She has always sat in the back of the classroom,\n"
	"reading books alone. Sometimes it is a hardcover that looks difficult,\n"
	"other times it is a comic book with a cover design indicating\n"
	"that it will decrease your intellect just by reading it.\n"
	" \n"
	"Suitably, she's smart and is at the top of the class\n"
	" \n"
	"You'd definitely find the name Hitagi Senjougahara\n"
	"within the top 10 names in the rankings\n"
	"passed out after an exam.\n"
	" \n"
	"She doesn't seem to have any friends.\n"
	"Not even one person.\n"
	" \n"
	"Of course, on the other hand, you can't say that she's being bullied.\n"
	"Senjougahara is always, with a face like it's natural, there, in the back of the class, reading a book.\n"
	"She was building a wall around herself.\n"
};
static const size_t zlib_uncompressed_size = 1023;


TEST_CASE( "zlib Compression", "[zlib]" ) {
	// zlib_t zlib{zlib_mode::deflate};
	// uint8_t output_buffer[2_KiB]{};
	// size_t out = zlib.transform(zlib_uncompressed, zlib_uncompressed_size, output_buffer);

	// // dump_hex(output_buffer, out);

	// REQUIRE(out == zlib_uncompressed_size);
}

TEST_CASE( "zlib Decompression", "[zlib]" ) {
	// zlib_t zlib{zlib_mode::inflate};
	// uint8_t output_buffer[2_KiB]{};
	// size_t out = zlib.transform(zlib_compressed, zlib_compressed_size, output_buffer);

	// // dump_hex(output_buffer, out);

	// REQUIRE(out == zlib_compressed_size);
}

/* utility.cc - Miscellaneous utilities */
#include <utility.hh>

/* Some basic byte swapping machinery */
constexpr uint16_t _sns_bswap16(const uint16_t x) noexcept {
	return (((x & 0x00FF) << 8) |
	        ((x & 0xFF00) >> 8));
}

constexpr uint32_t _sns_bswap32(const uint32_t x) noexcept {
	return (((x & 0x000000FF) << 24) |
	        ((x & 0x0000FF00) << 8 ) |
	        ((x & 0x00FF0000) >> 8 ) |
	        ((x & 0xFF000000) >> 24));
}

constexpr uint64_t _sns_bswap64(const uint64_t x) noexcept {
	return (((x & 0x00000000000000FF) << 56) |
	        ((x & 0x000000000000FF00) << 40) |
	        ((x & 0x0000000000FF0000) << 24) |
	        ((x & 0x00000000FF000000) << 8 ) |
	        ((x & 0x000000FF00000000) >> 8 ) |
	        ((x & 0x0000FF0000000000) >> 24) |
	        ((x & 0x00FF000000000000) >> 40) |
	        ((x & 0xFF00000000000000) >> 56));
}

#include <elf.hh>
#include <utility.hh>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>

TEST_CASE( "Enum bitmasks", "[utility]" ) {
	elf32_shflags_t test = elf32_shflags_t::Write | elf32_shflags_t::Alloc;

	REQUIRE((test & elf32_shflags_t::Write) == elf32_shflags_t::Write);
	REQUIRE((test & elf32_shflags_t::Alloc) == elf32_shflags_t::Alloc);
	REQUIRE((test & elf32_shflags_t::Merge) != elf32_shflags_t::Merge);

	test &= ~elf32_shflags_t::Alloc;
	REQUIRE((test & elf32_shflags_t::Alloc) != elf32_shflags_t::Alloc);

}

enum class test_enum : uint8_t {
	None,
	Test1,
	Test2,
	Test3 = 255,
};

const std::array<const enum_pair_t<test_enum>, 4> test_enum_s{{
	{ test_enum::None,  "None"  },
	{ test_enum::Test1, "Test1" },
	{ test_enum::Test2, "Test2" },
	{ test_enum::Test3, "Test3" },
}};

TEST_CASE( "enum and string mapping", "[utility]" ) {
	REQUIRE(std::strcmp(enum_name(test_enum_s, test_enum::Test1).c_str(), "Test1") == 0);
	REQUIRE(enum_value(test_enum_s, "Test3") == test_enum::Test3);
}

TEST_CASE( "User defined literals", "[utility]" ) {
	/* Kebibytes */
	REQUIRE(1_KiB == 1024);
	REQUIRE(2_KiB == 2048);
	REQUIRE(4_KiB == 4096);

	/* Mebibytes */
	REQUIRE(1_MiB == 1048576);
	REQUIRE(2_MiB == 2097152);
	REQUIRE(4_MiB == 4194304);

	/* Gibibytes */
	REQUIRE(1_GiB == 1073741824);
	REQUIRE(2_GiB == 2147483648);
	REQUIRE(4_GiB == 4294967296);

	/* Kilobytes */
	REQUIRE(1_KB == 1000);
	REQUIRE(2_KB == 2000);
	REQUIRE(4_KB == 4000);

	/* Megabytes */
	REQUIRE(1_MB == 1000000);
	REQUIRE(2_MB == 2000000);
	REQUIRE(4_MB == 4000000);

	/* Gigabytes */
	REQUIRE(1_GB == 1000000000);
	REQUIRE(2_GB == 2000000000);
	REQUIRE(4_GB == 4000000000);
}

TEST_CASE( "set_t structure test", "[utility]" ) {
	const uint8_t data[] = {
		0x71, 0x72, 0x64, 0x63, 0x84, 0x7f, 0xa8, 0xfd,
		0x48, 0x03, 0x01, 0x08, 0xe1, 0xda, 0xcb, 0x80,
		0x36, 0x4b, 0xd4, 0xa3, 0x47, 0x00, 0x21, 0x28,
		0x86, 0x67, 0x7a, 0xb3, 0xf0, 0x74, 0xe3, 0xc2
	};

	set_t test_set{&data, 32};

	REQUIRE(test_set.data() == &data);
	REQUIRE(test_set.size() == 32);

	// REQUIRE(test_set[15] == 0x80);
	// REQUIRE(test_set[31] == 0xc2);
}

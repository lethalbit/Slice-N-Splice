#include <cstdio>
#include <type_traits>
#include <algorithm>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <utility.hh>

enum class Flags : uint8_t {
	None   = 0,
	Foo    = 1 << 1,
	Bar    = 1 << 2,
	Baz    = 1 << 3,
	Qux    = 1 << 4,
	Quux   = 1 << 5,
	Corge  = 1 << 6,
	Grault = 1 << 7,
};

template<>
struct EnableBitmask<Flags>{
		static constexpr bool enabled = true;
};
const std::array<const enum_pair_t<Flags>, 8> flags_s{{
	{ Flags::None,    "No Flags" },
	{ Flags::Foo,     "Foo"      },
	{ Flags::Bar,     "Bar"      },
	{ Flags::Baz,     "Baz"      },
	{ Flags::Qux,     "Qux"      },
	{ Flags::Quux,    "Quux"     },
	{ Flags::Corge,   "Corge"    },
	{ Flags::Grault,  "Grault"   },
}};

TEST_CASE( "Flag extraction", "[utility]" ) {
	/* This test isn't the best as it's order-dependent for the flags */
	SECTION( "Raw flag extraction" ) {
		Flags tc1 = Flags::Foo | Flags::Baz | Flags::Grault;
		std::vector<Flags> tc1_exp = { Flags::Foo , Flags::Baz , Flags::Grault };

		auto retvec = extract_flags<Flags>(tc1, flags_s);

		REQUIRE(retvec.size() == tc1_exp.size());
		REQUIRE(retvec == tc1_exp);
	}

	SECTION( "Flag pair extraction" ) {
		Flags tc2 = Flags::Grault | Flags::Quux | Flags::Baz | Flags::Bar;
		std::vector<enum_pair_t<Flags>> tc2_exp = {
			{ Flags::Grault,  "Grault"   },
			{ Flags::Quux,    "Quux"     },
			{ Flags::Baz,     "Baz"      },
			{ Flags::Bar,     "Bar"      },
		};

		auto retvec = extract_flag_pairs<Flags>(tc2, flags_s);

		/*
			We need to reverse the returned vector because it's in-order
			while the test vector is inverted.
		*/
		std::reverse(retvec.begin(), retvec.end());

		REQUIRE(retvec.size() == tc2_exp.size());
		REQUIRE(std::equal(retvec.begin(),
					retvec.end(),
					tc2_exp.begin(),
					[](const enum_pair_t<Flags>& l, const enum_pair_t<Flags>& r){
						return (l.value() == r.value());
					}));
	}

	SECTION ( "Unknown Flag extraction" ) {
		auto ret = extract_flags<Flags>(Flags::None, flags_s);
		auto ret2 = extract_flag_pairs<Flags>(Flags::None, flags_s);

		REQUIRE(ret.size() == 1);
		REQUIRE(ret2.size() == 1);

		REQUIRE(ret[0] == Flags::None);

		REQUIRE(ret2[0].value() == Flags::None);
		REQUIRE(std::string{ret2[0].name()} == std::string{"No Flags"});
	}
}

TEST_CASE( "Byte Swapping" , "[utility]") {
	REQUIRE(_sns_bswap16(0xEDF3U) == 0xF3EDU);
	REQUIRE(_sns_bswap32(0x7B04D1BU) == 0x1B4DB007U);
	REQUIRE(_sns_bswap64(0x7F62B769CDF8AE44U) == 0x44AEF8CD69B7627FU);
}

TEST_CASE( "Units", "[utility]") {
	SECTION( "IEC Units" ) {
		REQUIRE(128_KiB == 131072);
		REQUIRE(1024_KiB == 1_MiB);

		REQUIRE(32_MiB == 33554432);
		REQUIRE(1024_MiB == 1_GiB);

		REQUIRE(512_GiB == 549755813888);
	}

	SECTION( "SI Units" ) {
		REQUIRE(128_KB == 128000);
		REQUIRE(1000_KB == 1_MB);

		REQUIRE(32_MB == 32000000);
		REQUIRE(1000_MB == 1_GB);

		REQUIRE(512_GB == 512000000000);
	}
}

TEST_CASE( "Enum bitwise operators", "[utility]" ) {
	SECTION ( "Bitwise Or" ) {
		REQUIRE(static_cast<uint8_t>(Flags::Foo    | Flags::Quux)  == ((1U << 1U) | (1U << 5U)));
		REQUIRE(static_cast<uint8_t>(Flags::Quux   | Flags::Foo)   == ((1U << 5U) | (1U << 1U)));
		REQUIRE(static_cast<uint8_t>(Flags::Grault | Flags::Corge) == ((1U << 7U) | (1U << 6U)));
		REQUIRE(static_cast<uint8_t>(Flags::Corge  | Flags::None)  == ((1U << 6U) | 0U));
	}

	SECTION ( "Bitwise And" ) {
		REQUIRE(static_cast<uint8_t>(Flags::Foo    & Flags::Quux)  == ((1U << 1) & (1U << 5U)));
		REQUIRE(static_cast<uint8_t>(Flags::Quux   & Flags::Foo)   == ((1U << 5) & (1U << 1U)));
		REQUIRE(static_cast<uint8_t>(Flags::Grault & Flags::Corge) == ((1U << 7) & (1U << 6U)));
		REQUIRE(static_cast<uint8_t>(Flags::Corge  & Flags::None)  == ((1U << 6) & 0U));
	}

	SECTION ( "Bitwise Not" ) {
		REQUIRE(static_cast<uint8_t>(~Flags::Foo   )  == static_cast<uint8_t>(~(1U << 1U)));
		REQUIRE(static_cast<uint8_t>(~Flags::Quux  )  == static_cast<uint8_t>(~(1U << 5U)));
		REQUIRE(static_cast<uint8_t>(~Flags::Grault)  == static_cast<uint8_t>(~(1U << 7U)));
		REQUIRE(static_cast<uint8_t>(~Flags::Corge )  == static_cast<uint8_t>(~(1U << 6U)));
	}

	SECTION ( "Bitwise Xor" ) {
		REQUIRE(static_cast<uint8_t>(Flags::Foo    ^ Flags::Quux)  == ((1U << 1U) ^ (1U << 5U)));
		REQUIRE(static_cast<uint8_t>(Flags::Quux   ^ Flags::Foo)   == ((1U << 5U) ^ (1U << 1U)));
		REQUIRE(static_cast<uint8_t>(Flags::Grault ^ Flags::Corge) == ((1U << 7U) ^ (1U << 6U)));
		REQUIRE(static_cast<uint8_t>(Flags::Corge  ^ Flags::None)  == ((1U << 6U) ^ 0U));
	}

	SECTION ( "Bitwise Or Equ" ) {
		Flags f{Flags::Foo};
		uint8_t r{1U << 1U};
		REQUIRE(static_cast<uint8_t>(f |= Flags::Quux) == (r |= (1U << 5U)));
	}

	SECTION ( "Bitwise And Equ" ) {
		Flags f{Flags::Foo};
		uint8_t r{1U << 1U};
		REQUIRE(static_cast<uint8_t>(f &= Flags::Quux) == (r &= (1U << 5U)));
	}

	SECTION ( "Bitwise Xor Equ" ) {
		Flags f{Flags::Foo};
		uint8_t r{1U << 1U};
		REQUIRE(static_cast<uint8_t>(f ^= Flags::Quux) == (r ^= (1U << 5U)));
	}
}

TEST_CASE( "Enum and string mapping", "[utility]" ) {
	REQUIRE(enum_name(flags_s, Flags::Quux) == std::string{"Quux"});
	REQUIRE(enum_value(flags_s, "Quux") == Flags::Quux);
}

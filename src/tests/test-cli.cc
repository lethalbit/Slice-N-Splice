#include <iostream>
#include <cstdio>

#include <catch2/catch.hpp>

#include <utility.hh>
#include <cli.hh>



static const char* faux_args[] = {
	"-t",
	"-q",
	"thing",
	"--long-opt",
	"c",
	"-n",
	"0x1B4DB007",
	"-e",
	"foo",
};

enum class EnumOpt : uint8_t {
	None,
	Foo,
	Bar,
	Baz,
	Qux,
};

TEST_CASE( "command parsing", "[cli]" ) {
	arg<bool> thing("t");
	arg<bool> DoThing("t", "thing", "Does the thing", true, false);
	arg<std::string> Qux("q", "qux", "Qux the Foo", true);
	arg<char> Long("", "long-opt", "Looooong option", false);
	arg<uint64_t> Number("n", "", "Set the number", false);
	// arg<EnumOpt> EnumOption("e", "enum", "Sets an enum", false);
	// EnumOption.validator([&](){
	// 	return true;
	// });

	REQUIRE(_args.size() == 5);

	// std::cout << static_cast<bool>(DoThing) << std::endl;

	// uint64_t parsed = ParseCli(8, faux_args);
	// REQUIRE(parsed == 0);
}


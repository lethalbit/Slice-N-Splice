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
	"0x1B4DB007"
};


TEST_CASE( "command parsing", "[cli]" ) {
	arg<bool> DoThing("t", "thing", "Does the thing", true, false);


	uint64_t parsed = ParseCli(7, faux_args);
	REQUIRE(parsed == 0);
}


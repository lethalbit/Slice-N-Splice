#include <cstdio>
#include <iostream>
#include <type_traits>


#include <catch2/catch.hpp>

#include <zlib.hh>
#include <elf.hh>
#include <utility.hh>


TEST_CASE("ELF64 File", "[elf]") {
	// elf64_t self{"../etc/test-files/test.elf64"};
}

TEST_CASE( "ELF Magic", "[elf]" ) {
	elf_magic_t magic{};
	REQUIRE(magic.is_valid() == false);
	magic.set();
	std::array<uint8_t, 4> _magic_array{0x7F, 'E', 'L', 'F'};
	REQUIRE(magic.get() == _magic_array);
	REQUIRE(magic.is_valid() == true);
}



TEST_CASE( "ELF File Test", "[elf]" ) {

	// auto self = elf_loader("/proc/self/exe");

	elf64_t self{"./test-objs/elf"};

	REQUIRE(self.valid() == true);

	REQUIRE(std::is_same<decltype(self), elf64_t>::value);

	REQUIRE(self.elf_valid() == true);

	REQUIRE(self.header().ident().eclass() == elf_class_t::ELF64);

	REQUIRE(self.header().ident().data() == elf_data_t::LSB);

	REQUIRE(self.header().ident().version() == elf_ident_version_t::Current);

	REQUIRE(self.header().ident().abi() == elf_osabi_t::Linux);

	REQUIRE(self.header().ident().abi_version() == 0);

	REQUIRE(self.header().type() == elf_type_t::Executable);

	REQUIRE(self.header().machine() == elf_machine_t::X86_64);

	REQUIRE(self.header().version() == elf_version_t::Current);

	REQUIRE(self.header().phoff() == 0);

	REQUIRE(self.header().phentsize() == 0);

	REQUIRE(self.header().shentsize() == 0);

	REQUIRE(self.header().phnum() == self.pheaders().size());

	REQUIRE(self.header().shnum() == self.sheaders().size());
}

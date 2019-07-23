#include <cstdio>
#include <iostream>
#include <type_traits>


#include <catch2/catch.hpp>

#include <zlib.hh>
#include <elf.hh>
#include <utility.hh>



TEST_CASE( "ELF File Test", "[elf]" ) {

	// auto self = elf_loader("/proc/self/exe");

	elf64_t self{"/proc/self/exe"};

	REQUIRE(std::is_same<decltype(self), elf64_t>::value);

	REQUIRE(self.is_valid() == true);

	REQUIRE(self.header().ident().eclass() == elf_class_t::ELF64);

	REQUIRE(self.header().ident().data() == elf_data_t::LSB);

	REQUIRE(self.header().ident().version() == elf_ident_version_t::Current);

	REQUIRE(self.header().ident().abi() == elf_osabi_t::Linux);

	REQUIRE(self.header().ident().abi_version() == 0);

	REQUIRE(self.header().type() == elf_type_t::Executable);

	REQUIRE(self.header().machine() == elf_machine_t::X86_64);

	REQUIRE(self.header().version() == elf_version_t::Current);

	REQUIRE(self.header().phoff() == 64);

	REQUIRE(self.header().phentsize() == sizeof(elf64_phdr_t));

	REQUIRE(self.header().shentsize() == sizeof(elf64_shdr_t));

	REQUIRE(self.header().phnum() == self.pheaders().length());

	REQUIRE(self.header().shnum() == self.sheaders().length());
}

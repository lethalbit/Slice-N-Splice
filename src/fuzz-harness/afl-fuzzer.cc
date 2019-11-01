/* SNS AFL fuzzer harness */

#include <zlib.hh>
#include <utility.hh>
#include <iostream>

#if defined(_fuzz_target_elf)
#include <elf.hh>
#endif

int main(int argc, char** argv) {


#if defined(_fuzz_target_elf)
	elf64_t elf{argv[1]};
	std::cout << "CLASS: " << elf.header().ident().eclass() << "\n";
	std::cout << "DATA: " << elf.header().ident().data() << "\n";
	std::cout << "VERSION: " << elf.header().ident().version() << "\n";
	std::cout << "ABI: " << elf.header().ident().abi() << "\n";
	std::cout << "ABI VERSION: " << elf.header().ident().abi_version() << "\n";
	std::cout << "TYPE: " << elf.header().type() << "\n";
	std::cout << "MACHINE: " << elf.header().machine() << "\n";
	std::cout << "VERSION: " << elf.header().version() << "\n";
	std::cout << "ENTRY: " << elf.header().entry() << "\n";
	std::cout << "PHOFF: " << elf.header().phoff() << "\n";

#endif

	return 0;
}

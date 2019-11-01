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



#endif

	return 0;
}

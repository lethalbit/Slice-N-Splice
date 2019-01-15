project(
	'Slice N Splice',
	'cpp',
	default_options: [
		'cpp_std=gnu++17',
		'warning_level=3'
	],
	version: '0.1'
)

cxx = meson.get_compiler('cpp')



if cxx.has_header('experimental/filesystem') == false
	error('Unable to find C++17 header experimental/filesystem')
endif

deps = [
	dependency('zlib', required: true),
	dependency('ncursesw', required: true, version: '>=6.0.20160213'),
	cxx.find_library('libstdc++fs', required: true)
]

srcs = [
	'src/elf.cc',
	'src/macho.cc',
	'src/pe.cc',
	'src/coff.cc',
	'src/zlib.cc',
]

executable(
	'sns',
	'src/sns-tui.cc',
	srcs,
	include_directories: [ include_directories('src/include') ],
	dependencies: deps,
	install: true,
)

install_man('doc/sns.1')

# ==== TEST HARNESS ==== #

test_srcs = [
	'src/tests/test-main.cc',
	'src/tests/test-elf.cc',
	'src/tests/test-macho.cc',
	'src/tests/test-pe.cc',
	'src/tests/test-coff.cc',
]

catch_harness = executable(
	'sns-test',
	srcs,
	test_srcs,
	include_directories: [ include_directories('src/include', 'third_party') ] ,
	dependencies: [ deps ],
	install: false,
)
test('Catch2 test', catch_harness)

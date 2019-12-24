project(
	'Slice N Splice',
	'cpp',
	default_options: [
		'cpp_std=gnu++17',
		'warning_level=3',
	],
	version: '0.1'
)

cxx = meson.get_compiler('cpp')
mkobj = find_program('./etc/mkobj.sh', required: true)

deps = [
	dependency('zlib', required: true),
	dependency('ncursesw', required: true, version: '>=6.0.20160213')
]

srcs = [
	'src/aout.cc',
	'src/cli.cc',
	'src/coff.cc',
	'src/ecoff.cc',
	'src/elf.cc',
	'src/macho.cc',
	'src/os360.cc',
	'src/pe.cc',
	'src/utility.cc',
	'src/xcoff.cc',
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

	'src/tests/test-cli.cc',
	'src/tests/test-coff.cc',
	'src/tests/test-ecoff.cc',
	'src/tests/test-elf.cc',
	'src/tests/test-fd_t.cc',
	'src/tests/test-macho.cc',
	'src/tests/test-mmap_t.cc',
	'src/tests/test-os360.cc',
	'src/tests/test-pe.cc',
	'src/tests/test-span.cc',
	'src/tests/test-utility.cc',
	'src/tests/test-xcoff.cc',
	'src/tests/test-zlib.cc',

]

obj_srcs = [
	'etc/test-files/aout.S',
	'etc/test-files/coff.S',
	'etc/test-files/ecoff.S',
	'etc/test-files/elf.S',
	'etc/test-files/macho.S',
	'etc/test-files/os360.S',
	'etc/test-files/pe.S',
	'etc/test-files/xcoff.S',
]

foreach obj_src : obj_srcs
	custom_target(
		'@0@'.format(obj_src.underscorify()),
		input: obj_src,
		output: '@BASENAME@',
		command: [mkobj, '@INPUT@', './test-objs/@OUTPUT@'],
		build_by_default: true
	)
endforeach

catch_harness = executable(
	'sns-test',
	srcs,
	test_srcs,
	include_directories: [ include_directories('src/include', 'third_party') ] ,
	dependencies: [ deps ],
	install: false,
)
test('Catch2 test', catch_harness)

if get_option('enable_fuzzing')
	obj_fmts = [ 'aout', 'coff', 'ecoff', 'elf', 'macho', 'os360', 'pe', 'xcoff' ]
	foreach fmt : obj_fmts
		if get_option('fuzz_@0@'.format(fmt))
			message('Enabling fuzz target for @0@ objects'.format(fmt))
			obj_fuzzer = executable('@0@-fuzz-harness'.format(fmt),
				'src/fuzz-harness/afl-fuzzer.cc',
				'src/utility.cc',
				'src/zlib.cc',
				'src/@0@.cc'.format(fmt),
				cpp_args: [
					'-D_fuzz_target_@0@'.format(fmt)
				],
				include_directories: [ include_directories('src/include', 'third_party') ] ,
				dependencies: [ deps ]
			)

			run_target('fuzz-@0@'.format(fmt),
				command: ['etc/afl-fuzz.sh', obj_fuzzer.full_path()],
				depends: obj_fuzzer
			)
			message('Added \'fuzz-@0@\' target'.format(fmt))
		else
			message('Skipping fuzz target for @0@ objects'.format(fmt))
		endif
	endforeach
endif


run_target('lint', command: ['etc/lint.sh'])

if get_option('valgrind_checks')
	valgrind = find_program('valgrind', required: true)

	common_args = [
		'--track-fds=yes',
		'--demangle=yes',
		'--num-callers=50',
		'--error-limit=no',
		'--keep-debuginfo=yes',
		'--show-error-list=yes',
		'--read-inline-info=yes',
		'--read-var-info=yes',
		'--trace-children=yes',
		'--run-libc-freeres=yes',
		'--run-cxx-freeres=yes',
		'--fair-sched=yes',
	]


	memcheck = run_target('memcheck',
		command: [
			valgrind,
			'--tool=memcheck',
			common_args,
			'--xml=yes',
			'--xml-file=@0@/sns-test.%p.err.xml'.format(meson.build_root()),
			'--leak-check=full',
			'--show-leak-kinds=all',
			'--leak-resolution=high',
			'--leak-check-heuristics=all',
			'--xtree-leak=yes',
			'--xtree-leak-file=@0@/sns-test.%p.kcg'.format(meson.build_root()),
			'--undef-value-errors=yes',
			'--track-origins=yes',
			'--expensive-definedness-checks=yes',
			catch_harness
		],
		depends: catch_harness
	)

	callgrind = run_target('callgrind',
		command: [
			valgrind,
			'--tool=callgrind',
			common_args,
			'--dump-line=yes',
			'--collect-jumps=yes',
			'--collect-systime=yes',
			'--collect-bus=yes',
			'--separate-threads=yes',
			'--cache-sim=yes',
			'--branch-sim=yes',
			'--simulate-wb=yes',
			'--cacheuse=yes',
			'--callgrind-out-file=@0@/sns-test.%p.cg'.format(meson.build_root()),
			catch_harness
		],
		depends: catch_harness
	)

	massif = run_target('massif',
		command: [
			valgrind,
			'--tool=massif',
			common_args,
			'--heap=yes',
			'--stacks=yes',
			'--depth=50',
			'--massif-out-file=@0@/sns-test.%p.mf'.format(meson.build_root()),
			catch_harness
		],
		depends: catch_harness
	)
endif

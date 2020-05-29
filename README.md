# Slice 'N Splice [![Build Status](https://travis-ci.org/lethalbit/Slice-N-Splice.svg?branch=master)](https://travis-ci.org/lethalbit/Slice-N-Splice) [![codecov](https://codecov.io/gh/lethalbit/Slice-N-Splice/branch/master/graph/badge.svg)](https://codecov.io/gh/lethalbit/Slice-N-Splice) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/lethalbit/Slice-N-Splice.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/lethalbit/Slice-N-Splice/context:cpp)
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Flethalbit%2FSlice-N-Splice.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Flethalbit%2FSlice-N-Splice?ref=badge_shield)


Slice 'N Splice (SNS) is an application that encourages gross abuse of object files in all their forms, mix and match them as you please.

In addition to the mangling of object files, Slice 'N Splice can be used almost like ffmpeg, but targeting object files, from simpling extracting symbol tables and compressing sections in-place, to converting ELF relocation data into XCOFF relocation information.

## Usage
SNS is primarily used via its interactive Ncurses interface, that can be reached by simply running `sns` or `sns <filename>`. However, an interactive interface may not always be the best choice, so SNS also has an extensive set of command line options that allow you to do almost everything you could via the interface, they are documented in the [`sns(1)`](doc/sns.1) manpage.


## Building
To build SNS, you need the following tools and libraries.
 
 * Meson
 * Ninja
 * Zlib
 * Ncurses

You will also need a C++ 17 compliant compiler. 


You can then build SNS with the following commands:

```
$ meson build
...
$ ninja -C build
```

To run the tests, use `ninja -C build test`

To install, you can run the following:
```
$ DESTDIR=/usr/local/ ninja install
```



## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Flethalbit%2FSlice-N-Splice.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Flethalbit%2FSlice-N-Splice?ref=badge_large)
# Slice 'N Splice
Slice 'N Splice (SNS) is an application that encourages gross abuse of the ELF file format by allowing one to do whatever one sees fit with the file within the limits of the standard and beyond.

## Usage
SNS is primarily used via its interactive Ncurses interface, that can be reached by simply running `sns` or `sns <filename>`. However, an interactive interface may not always be the best choice, so SNS also has an extensive set of command line options that allow you to do almost everything you could via the interface, they are documented in the [`sns(1)`](doc/sns.1)] manpage


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


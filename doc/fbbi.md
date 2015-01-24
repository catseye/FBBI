FBBI: The Flaming Bovine Befunge-98 Interpreter
===============================================

Version 1.0. Copyright ©1998-2011 Cat's Eye Technologies. All rights
reserved.

Introduction
------------

FBBI is an interpreter for the Befunge-98 language as defined by the
[Funge-98 Final Specification](http://catseye.tc/projects/funge98/),
written in 100% ANSI C, and placed under a BSD-style license.

### Funge-98 Interpreter Information

FBBI's handprint is 0x46424249 ('FBBI'). FBBI v1.0 implements the
fingerprints [0x4e554c4c
NULL](http://catseye.tc/projects/funge98/library/NULL.html) and
[0x524f4d41 ROMA](http://catseye.tc/projects/funge98/library/ROMA.html).

### Usage

See the header comment of `fbbi.c` for definitive information about
usage.

### History

This is version 1.0 of the Flaming Bovine Befunge-98 Interpreter. See
the header comment of `fbbi.c` for definitive version history
information.

### Conformancy and Implementation Notes

Due to the nature of reality, this list may not be complete.

-   Modulo the lovely ambiguities in the Funge-98 Final Specification,
    this version of FBBI appears to conform to it.
-   Perhaps more materially, this version of FBBI substantially passes
    the [Mycology](http://users.tkk.fi/~mniemenm/befunge/mycology.html)
    test suite. See the header comment of `fbbi.c` for detailed
    information.
-   Because FBBI is written in ANSI C, end-of-line sequences which
    consist of a single CR may not be readable on MS-DOS machines.
    FBBI's strict ANSI policy can be undefined in `config.h`, in which
    case it should compile to use binary file mode under MS-DOS.
-   The stack stack, and each stack, is of a user-specified size, not
    dynamic.
-   `A-Z` semantic stacks are limited to 26 overloads each.
-   `h`, `l`, and `m` are all Trefunge-specific, and thus are not
    implemented in FBBI, because FBBI only implements Befunge-98.
-   FBBI does not implement Concurrent Funge, so `t` is not implemented
    either.
-   The C source code for the interpreter should be 100% ANSI C (except
    where noted above in regards to CR as a sole end-of-line marker on
    MS-DOS systems.)
-   Compliation of this version of FBBI has been tested only under gcc
    4.4.5 on Ubuntu 10.10.

### Included Befunge-98 Example Programs

-   `eg/binary.b98`

    Tests to see if reading a file in, binary-mode, actually works.

-   `eg/finger.b98`

    Demonstrates NULL and ROMA fingerprints with `(` and `)`.

-   `eg/hello.b98`

    Demonstrates SGML-style spaces in stringmode.

-   `eg/randdna.b98`

    Perfect for synthesizing random organisms!

-   `eg/script.b98`

    Demonstrates FBBI's -script command line option.

-   `eg/cgi.b98.cgi`

    Another Befunge-98 script; this one should run as a CGI.

-   `eg/trim-out.b98`

    Tests to see if writing a files strips trailing spaces and newlines.

-   `eg/under.b98`

    Test for `u` and storage offset.

### Acknowledgements

Thanks to Sam Holden for providing the initial version of the Makefile.

Thanks to Jeffrey Lee for providing his bug-fixing patches.

Thanks to all those on the Befunge and/or Esolang mailing lists for
their bug reports, the fixes for which made their way into patches.

Thanks to Matti Niemenmaa for writing the Mycology Befunge-98 test
suite.

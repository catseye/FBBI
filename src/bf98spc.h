/*
   bf98spc.h for bf98spc.c v1.0 May 2011 Chris Pressey
   Dynamic Befunge-Space ANSI C Functions - header.

   Copyright (c)1998-2011 Cat's Eye Technologies.  All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notices, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notices, this list of conditions, and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. Neither the names of the copyright holders nor the names of their
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission. 

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
   COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef _BF98SPC_H_
#define _BF98SPC_H_

#ifndef _WIN32
  #ifndef __FreeBSD__
    #ifndef __NetBSD__
      #ifndef __CYGWIN__
        #include <values.h>
      #endif
    #endif
  #endif
#endif

#ifndef MAXLONG
  #define MAXLONG ((long)0x7fffffff)
#endif

/* The engine allocates the playfield in chunks of
   2^SNIPPET_BITS x 2^SNIPPET_BITS.  4 seems to be a good
   value (16x16x4=1024=1kbyte.)  Although you'll want to
   take this down if storage is unusually sparse,
   and bump it up if it's unusually compact. */

#define SNIPPET_BITS 4

#define SNIPPET_SIZE (1 << SNIPPET_BITS)
#define SNIPPET_MASK (SNIPPET_SIZE - 1)

/* internal storage definition of a 'snippet' (nxn chunk of space). */
typedef struct Snippet
{
  signed long int space[SNIPPET_SIZE * SNIPPET_SIZE];
} snippet;

/* internal storage definition of a key tree node or leaf. */
/* each node points to a single snippet of the playfield */
typedef struct KeyTree
{
  signed long int key_x;
  signed long int key_y;
  snippet * data;
  struct KeyTree * left;  /* xys less than xy */
  struct KeyTree * right; /* xys greater than xy */
} keytree;

typedef struct BefungeSpace
{
  signed long int max_x, max_y, min_x, min_y;
  keytree * root;
} bfspace;

extern bfspace *     bfspace_alloc (bfspace * p);
extern signed long int bfspace_fetch (bfspace * p, signed long int x, signed long int y);
extern int             bfspace_store (bfspace * p, signed long int x, signed long int y, signed long int data);
extern int             bfspace_inbounds (bfspace * p, signed long int x, signed long int y);
extern void            bfspace_free  (bfspace * p);
extern int             bfspace_fread (bfspace * p, FILE * f, signed long int x, signed long int y,
                        signed long int * w, signed long int * h, unsigned long int flags);
extern int	       bfspace_fwrite (bfspace * p, FILE * f, signed long int x, signed long int y,
			signed long int w, signed long int h, unsigned long int flags);

#endif  /* _BF98SPC_H_ not defined */

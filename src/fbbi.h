/*
   fbbi.h for fbbi.c v1.0 May 2011 Chris Pressey
   Flaming Bovine Befunge Interpreter - header.

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

#ifndef _FBBI_H_
#define _FBBI_H_

/*-- VERSION */

#define FBBI_VERSION_MAJOR  1
#define FBBI_VERSION_MINOR  0

#ifdef _WIN32
  #define FBBI_WIN32
#endif

#ifdef __MSDOS__
  #define FBBI_MSDOS
#endif

#ifdef FBBI_DEBUG
  #define DEBUG(x) fprintf(stderr, "<<%s>>\n", x); fflush(stderr);
  #else
  #define DEBUG(x)
#endif

/*-- TYPE DEFINES */

typedef signed long int cell;
typedef int             flag;

/*-- UTILITY DEFINES */

#define PRINT(x)     (((x >= 32) && (x <= 126)) ? (char)x : '.')
#define GET_a(i)     i->a = ip_pop(i);
#define GET_b(i)     i->b = ip_pop(i);
#define GET_ab(i)    GET_a(i) GET_b(i)
#define DELTA(i,x,y) i->dx = x; i->dy = y;
#define NEG(x)       x = x * -1;

/*-- EXTERNS */

#ifndef FBBI_MINIMAL
extern flag trace;
extern flag warn;
extern flag fast;
extern flag b93;
extern flag une;
#endif

extern char * bargs;
extern char * benv;

#endif /* _FBBI_H_ not defined */

/*-- END OF fbbi.h */

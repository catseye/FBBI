/*
   f98fp.h for f98fp.c v1.0 May 2011 Chris Pressey
   Funge-98 Fingerprints - header.

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

#ifndef _F98FP_H_
#define _F98FP_H_

extern cell get_fingerprint(ip * i);

#ifndef FBBI_MINIMAL

/* save the semantics of instruction c */
extern void save_semantics(ip * i, char c);

/* restore the last saved semantics of c */
extern void restore_semantics(ip * i, char c);

/* bind c to b() */
extern void bind_semantics(ip * i, char c, void (*b)(ip *));

/* save c, bind c to b() */
extern void overload_semantics(ip * i, char c, void (*b)(ip *));

/* load a fingerprint */
extern int  fingerprint_semantics(ip * i, cell fp, int load);

#endif

#endif /* _F98FP_H_ not defined */

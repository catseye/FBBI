/*
   fp/index.h for f98fp.c
   Lists fingerprints installed in this build of FBBI.
   v0.98 Oct 1 1998 Chris Pressey
   v0.98a Mar 26 2003 Chris Pressey

   Copyright (c)1998, 2001, 2003, Cat's Eye Technologies.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

     Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

     Neither the name of Cat's Eye Technologies nor the names of its
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission. 

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
   CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
   OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
   OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE. 

*/

/*
   to add a fingerprint:
   - add 'extern void semantics_zzzz(ip * i, int load);' prototype
   - add '{0xzzzzzzzz, semantics_zzzz},' in flib library[] =
   - compile fp/zzzz.c and recompile FBBI with fp/zzzz.obj
*/

#ifndef FBBI_MINIMAL

extern void semantics_NULL(ip * i, int load);
extern void semantics_ROMA(ip * i, int load);

typedef struct FungeLibrary
{
  cell fingerprint;
  void (*semantics)(ip *, int);
} flib;

flib library[] = 
{
  { 0x524f4d41, semantics_ROMA },
  { 0x4e554c4c, semantics_NULL },
  { 0x00000000, NULL }            /* should always be listed last */
};

#endif

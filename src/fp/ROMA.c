/*
   fp/ROMA.c for f98fp.c
   Funge-98 Roman Numerals Fingerprint
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

#include <stdio.h>
#include "../fbbi.h"
#include "../bf98spc.h"
#include "../f98stack.h"
#include "../f98ip.h"
#include "../f98i.h"
#include "../f98fp.h"

void fpi_roman(ip * i)
{
  switch (i->ir)
  {
    case 'C': ip_push(i, 100); break;
    case 'D': ip_push(i, 500); break;
    case 'I': ip_push(i, 1); break;
    case 'L': ip_push(i, 50); break;
    case 'M': ip_push(i, 1000); break;
    case 'V': ip_push(i, 5); break;
    case 'X': ip_push(i, 10); break;
    default: ip_reverse(i); break;
  }
}

void semantics_ROMA(ip * i, int load)
{
  if (load)
  {
    overload_semantics(i, 'C', &fpi_roman);
    overload_semantics(i, 'D', &fpi_roman);
    overload_semantics(i, 'I', &fpi_roman);
    overload_semantics(i, 'L', &fpi_roman);
    overload_semantics(i, 'M', &fpi_roman);
    overload_semantics(i, 'V', &fpi_roman);
    overload_semantics(i, 'X', &fpi_roman);
  } else
  {
    restore_semantics(i, 'C');
    restore_semantics(i, 'D');
    restore_semantics(i, 'I');
    restore_semantics(i, 'L');
    restore_semantics(i, 'M');
    restore_semantics(i, 'V');
    restore_semantics(i, 'X');
  }
}

/*-- END OF ROMA.c */

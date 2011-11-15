/*
   fp/NULL.c for f98fp.c
   Funge-98 Null Fingerprint.
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

void semantics_NULL(ip * i, int load)
{
  char c;
  if (load)
  {
    for(c='A';c<='Z';c++)
      overload_semantics(i, c, &ip_reverse);
  } else
  {
    for(c='Z';c>='A';c--)
      restore_semantics(i, c);
  }
}

/*-- END OF NULL.c */

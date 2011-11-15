/*
   f98fp.c for fbbi.c v1.0 May 2011 Chris Pressey
   Funge-98 Fingerprints - source.

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

#include <stdio.h>

#include "fbbi.h"
#include "bf98spc.h"
#include "f98ip.h"
#include "f98i.h"
#include "f98fp.h"
#include "fp/index.h"

cell get_fingerprint(ip * i)
{
  i->b = 0;
  i->a = ip_pop(i);
  for(;i->a > 0;i->a--)
  {
    i->b <<= 8;
    i->b += ip_pop(i);
  }
  return i->b;
}

#ifndef FBBI_MINIMAL

void save_semantics(ip * i, char c) /* save the semantics of instruction c */
{
  if ((c >= 'A') && (c <= 'Z')) /* bugfix 2003.0726 Ensure semantics are saved */
    i->fingins[c-'A'][++i->finglev[c-'A']] = instable[c-' '];
}

void restore_semantics(ip * i, char c) /* restore the last saved semantics of c */
{
  if ((c >= 'A') && (c <= 'Z') && (i->finglev[c-'A'] > 0))
    instable[c-' '] = i->fingins[c-'A'][i->finglev[c-'A']--];
}

void bind_semantics(ip * i, char c, void (*b)(ip *)) /* bind c to b() */
{
  instable[c-' '] = b;
  i = i;
}

void overload_semantics(ip * i, char c, void (*b)(ip *)) /* save c, bind c to b() */
{
  save_semantics(i, c);
  bind_semantics(i, c, b);
}

int fingerprint_semantics(ip * i, cell fp, int load)
{
  int k=0;
  if (fp==0) return 0;
  while(1)
  {
    if (library[k].fingerprint == fp)
    {
      library[k].semantics(i, load);
      return 1;
    }
    if (!library[k++].fingerprint) break;
  }
  return 0;
}

#endif

/*
   f98ip.h for f98ip.c v1.0 May 2011 Chris Pressey
   FBBI's Funge-98 Instruction Pointer - header.

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

#ifndef _F98IP_H_
#define _F98IP_H_

#include "f98stack.h"

typedef struct InstructionPointer
{
  cell ir;      /* instruction register */
  cell x, y;    /* position */
  cell dx, dy;  /* delta */
  cell sx, sy;  /* offset */
  cell a, b;    /* scratch storage */
  cell ec;      /* error code */
  flag sm;      /*-- stringmode */
  flag pm;      /*-- spacemode */
  flag hm;      /*-- haltmode */
  bfspace * bs; /* befunge-space */
  stack * s2;   /* stack-stack */
  void (*fingins[26][26])(struct InstructionPointer *);
  int finglev[26]; /* fingerprints */
  int ssize;
  int sssize;
} ip;

extern ip * ip_alloc(bfspace * bs, int ssize, int sssize);
extern void ip_free(ip * i);
extern void ip_reverse(ip * i);
extern void ip_march(ip * i);
extern void ip_backtrack(ip * i);
extern void ip_advance(ip * i);
extern void ip_move(ip * i);

stack * ip_stack(ip * i);
cell ip_pop(ip * i);
int ip_push(ip * i, cell x);

int ip_clearstack(ip * i);
cell ip_pop_soss(ip * i);
void ip_push_soss(ip * i, cell value);
int ip_allocstack(ip * i);
void ip_freestack(ip * i);
int ip_transfer(ip * i, int val);
void ip_pop_string(ip * i, char * s);
void ip_push_string(ip * i, char * s);
int ip_stack_measure(ip * i);
int ip_stacks_measure(ip * i);
int ip_stack_measure_offset(ip * i, int offset);
cell ip_stack_peek(ip * i, int offset);
cell ip_stack_peek_peek(ip * i, int o1, int o2);

/* Display a stack, from top to bottom, on stdout */
extern void         stack_dump(stack * s);

#endif /* _F98IP_H_ not defined */

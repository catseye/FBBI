/*
   f98ip.c for fbbi.c v1.0 May 2011 Chris Pressey
   FBBI's Funge-98 Instruction Pointer - source.

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
#include <stdlib.h>

#include "fbbi.h"
#include "bf98spc.h"
#include "f98stack.h"
#include "f98ip.h"

ip * ip_alloc(bfspace * bs, int ssize, int sssize)
{
  int k, j;
  ip * i;
  stack *initial_stack;
  i = (ip *)malloc(sizeof(ip));
  if (i != NULL)
  {
    i->sm=i->pm=i->hm=0; /* initfields */ i->dx=1;
    i->ir=i->y=i->dy=i->sx=i->sy=i->a=i->b=0;i->x=-1;
    i->bs = bs; i->ssize = ssize; i->sssize = sssize;
    i->s2 = malloc(sizeof(stack));
    if (i->s2 == NULL) return NULL;
    stack_init(i->s2, sssize, sizeof(stack));
    initial_stack = stack_push_placeholder(i->s2);
    if (initial_stack == NULL) return NULL;
    stack_init(initial_stack, ssize, sizeof(cell));
    for (k=0;k<26;k++)
    {
      i->finglev[k] = 0;
      for (j=0;j<26;j++) i->fingins[k][j] = NULL;
    }
  }
  return i;
}

void ip_free(ip * i)
{
  stack * x;
  while(stack_measure(i->s2))
  {
    x = stack_pop(i->s2);
    if (x != NULL) stack_release(x);
  }
  DEBUG("Freeing the stack stack");
  stack_release(i->s2);
  free(i->s2);
  DEBUG("Freeing the ip struct");
  free(i);
}

void ip_reverse(ip * i)
{
  NEG(i->dx);  NEG(i->dy);
}

void ip_march(ip * i)
{
  i->x += i->dx;
  i->y += i->dy;
}

void ip_backtrack(ip * i)
{
  ip_reverse(i);
  while (!bfspace_inbounds(i->bs, i->x, i->y))
    ip_march(i); /* bugfix 2003.0726 March back until inside fungespace to cope with edge-of-space #, j, etc. */

  while (bfspace_inbounds(i->bs, i->x, i->y))
    ip_march(i);

  ip_reverse(i);
  ip_march(i);
}

void ip_advance(ip * i)
{
  ip_march(i);
  if (!bfspace_inbounds(i->bs, i->x, i->y))
    ip_backtrack(i);
}

void ip_skipto(ip * i, cell target)
{
  ip_advance(i);
  while (bfspace_fetch(i->bs, i->x, i->y) != target)
  {
    ip_advance(i);
    #ifndef FBBI_MINIMAL
    if (!fast) printf(" \b");
    #endif
  }
}

void ip_move(ip * i)
{
  cell c;
  ip_advance(i);
  c = bfspace_fetch(i->bs, i->x, i->y);
  while (c == (cell)';' || c == (cell)' ')
  {
    if (c == (cell)';')
    {
      if (i->sm) return;      
      ip_skipto(i, (cell)';');
      ip_advance(i);
    }
    else while (bfspace_fetch(i->bs, i->x, i->y) == (cell)' ')
    {
  #ifndef FBBI_MINIMAL
      if (((i->sm) && (i->pm == 0)) || (b93))
  #else
      if ((i->sm) && (i->pm == 0))
  #endif
      {
  #ifndef FBBI_MINIMAL
        if (!b93)
  #endif
          i->pm = 1;
        return;
      }
      ip_advance(i);
  #ifndef FBBI_MINIMAL
      if (!fast) printf(" \b");
  #endif
      if ((bfspace_fetch(i->bs, i->x, i->y) == (cell)';') && (!i->sm))
      {
        ip_skipto(i, (cell)';'); ip_advance(i);
      }
    }
    c = bfspace_fetch(i->bs, i->x, i->y);
  }
  i->pm = 0;
}

/* might return NULL */
stack * ip_stack(ip * i)
{
  return (stack *)stack_read(i->s2, 0);
}

cell ip_pop(ip * i)
{
  cell * x;
  stack * s = (stack *)ip_stack(i);
  if (s != NULL)
  {
    x = (cell *)stack_pop(s);
    if (x != NULL) return *x; else return (cell)0;
  } else return (cell)0;
}

int ip_push(ip * i, cell x)
{
  stack * s = (stack *)stack_read(i->s2, 0);

  if (s != NULL)
  {
    stack_push(s, &x); 
    return 1;
  } else return 0;
}

int ip_clearstack(ip * i)
{
  stack * s = ip_stack(i);
  if (s != NULL)
  {
    stack_clear(s); 
    return 1;
  } else return 0;
}

cell ip_pop_soss(ip * i)
{
  cell *x;
  stack *soss = (stack *)stack_read(i->s2, 1);
  x = stack_pop(soss);
  if (x != NULL) return *(cell *)x; else return 0;
}

void ip_push_soss(ip * i, cell value)
{
  stack *soss = (stack *)stack_read(i->s2, 1);
  stack_push(soss, &value);
}

int ip_allocstack(ip * i)
{
  stack * u;
  u = stack_push_placeholder(i->s2);
  if (u == NULL) return 0;
  return stack_init(u, i->ssize, sizeof(cell));
}

void ip_freestack(ip * i)
{
  stack * x = stack_pop(i->s2);
  if (x != NULL) stack_release(x);
}

int ip_transfer(ip * i, int val)
{
  cell zero = (long)0;
  return stack_stack_transfer(i->s2, val, &zero);
}

void ip_pop_string(ip * i, char * s)
{
  stack_pop_string(stack_stack_top(i->s2), s);
}

void ip_push_string(ip * i, char * s)
{
  stack_push_string(stack_stack_top(i->s2), s);
}

int ip_stack_measure(ip * i)
{
  return stack_measure(stack_stack_top(i->s2));
}

int ip_stacks_measure(ip * i)
{
  return stack_measure(i->s2);
}

int ip_stack_measure_offset(ip * i, int offset)
{
  stack * s = stack_read(i->s2, offset);
  if (s != NULL) return stack_measure(s); else return 0;
}

cell ip_stack_peek(ip * i, int offset)
{
  cell * x;
  stack * s = stack_stack_top(i->s2);
  if (s != NULL)
  {
    x = stack_read(s, offset);
    if (x != NULL) return *(cell *)x; else return 0;
  } else return 0;
}

cell ip_stack_peek_peek(ip * i, int o1, int o2)
{
  cell * x;
  stack * s = (stack *)stack_read(i->s2, o1);
  if (s != NULL)
  {
    x = stack_read(s, o2);
    if (x != NULL) return *(cell *)x; else return (cell)0;
  } else return 0;
}

void stack_dump (stack * s)
{
  cell *x; int o;
  printf("Size %d: (Top: ", stack_measure(s));
  for (o = 0; o < stack_measure(s); o++) {
      x = stack_read(s, o);
      printf("%ld ", *x);
  }
  printf(")\n");
}

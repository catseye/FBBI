/*
   f98stack.c for fbbi.c v1.0 May 2011 Chris Pressey
   Stack ANSI C Functions - source.

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

/*
   Why use this package?
   - It's ANSI C
   - You need to store one or more stack stacks,
     as defined in Funge-98 (http://www.catseye.mb.ca/projects/funge98-+)

   Changes in v0.98:
   - FSS_DATATYPE is replaced with void * and dc (size in bytes)
   - stack stack is built upon stack with stack * as the cell type
*/

#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#include "f98stack.h"

/* Given the number of cells (sc) and bytes per cell (dc), configure a */
/* stack object and allocate the memory used for its backing store.  Note */
/* will only allocate the memory for the backing store of */
/* the stack -- it is up to the caller to allocate the actual */
/* struct Stack that manages the backing store.  (This allows */
/* the caller to choose to place it on e.g. another stack.) */
/* Returns 1 if the memory was successfully allocated, 0 otherwise. */
int stack_init(stack * u, long int sc, long int dc)
{
  if (u == NULL) return 0;
  u->mem = malloc(sc * dc);
  if (u->mem == NULL) return 0;
  u->sp = -1;
  u->sc = sc;
  u->dc = dc;
  SDEBUG("Successful Init", u);
  return 1;
}

/* Given a pointer to a stack, remove and return the top cell */
void * stack_pop(stack * s)
{
  SDEBUG("Before Pop", s);
  if (s->sp >= 0)
  {
    return ((char *)s->mem + (s->sp-- * s->dc));
  } else return NULL;
}

/* Given a pointer to a stack and a value, place it on the */
/* top of the stack */
int stack_push(stack * s, void * val)
{
  SDEBUG("Before Push", s);
  if(s->sp < s->sc-1)
  {
    memcpy(((char *)s->mem + (++s->sp * s->dc)), val, s->dc);
    SDEBUG("Successful Push", s);
    return 1;
  } else return 0;
}

/* Push a dummy value onto the top of the stack and return a */
/* pointer to it for further manipulation.  Returns NULL if no */
/* more space available on stack. */
void * stack_push_placeholder(stack * s)
{
  SDEBUG("Before Push Placeholder", s);
  if(s->sp < s->sc-1)
  {
    void * loc = NULL;
    ++s->sp;
    loc = ((char *)s->mem + (s->sp * s->dc));
    memset(loc, 0, s->dc);
    SDEBUG("Successful Push Placeholder", s);
    return loc;
  } else return NULL;
}

/* Pop a series of cells off the stack as chars until a 0 cell */
void stack_pop_string(stack * s, char * str)
{
  int i = 0;
  void * x = NULL;
  SDEBUG("Before Pop String", s);
  x = stack_pop(s);
  while ((s->sp >= 0) && (x != NULL) && (*(char *)x != 0))
  {
    str[i++] = *(char *)x;
    x = stack_pop(s);
  }
  if (s->sp == -1)
    str[i++] = *(char *)x; /* bugfix 2003.0726: EOS as string terminator */
  str[i] = (char)'\0';
}

/* Push a series of chars onto the stack as cells until a 0 char */
void stack_push_string(stack * s, char * str)
{
  int i = strlen(str) + 1;
  long int l = 0;
  SDEBUG("Before Push String", s);
  while (i > 0)
  {
    l = (long)0 | str[--i]; /* pad */
    stack_push(s, &l);
  }
}

/* Given a pointer to a stack and a number of cells from the top */
/* cell, return an indexed cell without removing it */
void * stack_read(stack * s, int offset /* 0 = TOS */ )
{
  void * k = NULL;
  if(offset < 0) offset *= -1;
  SDEBUG("Before Read", s);
  if (s->sp >= offset)
  {
    k = (void *)((char *)s->mem + (s->sp - offset) * s->dc);
    return k;
  } else return NULL;
}

/* Return the number of elements on a given stack */
int stack_measure(stack * s)
{
  SDEBUG("Before Measure", s);
  if (s != NULL) return s->sp + 1; else return 0;
}

/* Remove all cells from a given stack */
void stack_clear(stack * s)
{
  SDEBUG("Before Clear", s);
  if (s != NULL) s->sp = -1;
}

/* Free the backing store used by a stack, invalidating the stack. */
/* Like in stack_init, the memory of the Struct stack itself is not */
/* touched. */
void stack_release(stack * s)
{
  SDEBUG("Before Free", s);
  if (s != NULL && s->mem != NULL)
  {
    free(s->mem);
  }
}

/* Stack of stacks - transfer elements a la f98's {} instructions */
int stack_stack_transfer (stack * t, int count, void * zero)
{
  int i;
  stack * toss; stack * soss;
  void * q;

  SDEBUG("Before Transfer", t);
  if (stack_measure(t) > 1)
  {
    toss = (stack *)stack_read(t, 0);
    soss = (stack *)stack_read(t, 1);
    if (toss != NULL && soss != NULL)
    {
      if (count > 0) /* from SOSS to TOSS */
      {
        for(i=0;i<count;i++)
        {
          q = stack_read(soss, count - 1 - i);
          if (q != NULL) stack_push(toss, q); else stack_push(toss, zero);
        }
        soss->sp -= count;
        if (soss->sp < 0) soss->sp = -1;
      } else
      if (count < 0) /* from TOSS to SOSS */
      {
        count *= -1;
        for(i=0;i<count;i++)
        {
          q = stack_read(toss, count - 1 - i);
          if (q != NULL) stack_push(soss, q); else stack_push(soss, zero);
        }
        toss->sp -= count;
        if (toss->sp <= 0) toss->sp = -1;
      }
      return 1;
    }
  }
  return 0;
}

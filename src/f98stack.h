/*
   f98stack.h for f98stack.c v1.0 May 2011 Chris Pressey
   Funge-98 Stack Stack C Functions - header.

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

#ifndef _F98STACK_H_
#define _F98STACK_H_

typedef struct Stack
{
  void * mem;        /* pointer to array of cells of any type */
  long int sp;       /* stack pointer */
  long int sc;       /* total stack capacity */
  long int dc;       /* data capacity of each cell */
} stack;

#ifdef DEBUG_STACK
  #include <stdio.h>
  #ifndef SDEBUG
    #define SDEBUG(x, s) fprintf(stderr, "<-Stack:%s->\n", x);           \
                         fprintf(stderr, "memory: %08x\n", (unsigned int)s->mem);  \
                         fprintf(stderr, "stack pointer: %ld\n", s->sp);  \
                         fprintf(stderr, "stack capacity: %ld\n", s->sc);  \
                         fprintf(stderr, "data capacity: %ld\n", s->dc);    \
                         fflush(stderr);
    #endif
  #else
  #ifndef SDEBUG
    #define SDEBUG(x, s)
    #endif
#endif

/* Given the number of cells (sc) and bytes per cell (dc), */
/* configure a stack object and allocate its backing store */
extern int          stack_init(stack * u, long int sc, long int dc);

/* Given a pointer to a stack, remove and return the top cell */
extern void *       stack_pop(stack * s);

/* Given a pointer to a stack and a value, place it on the */
/* top of the stack */
extern int          stack_push(stack * s, void * val);

/* Push a dummy value onto the top of the stack and return a */
/* pointer to it for further manipulation */
extern void *       stack_push_placeholder(stack * s);

/* Pop a series of cells off the stack as chars until a 0 cell */
extern void         stack_pop_string(stack * s, char * str);

/* Push a series of chars onto the stack as cells until a 0 char */
extern void         stack_push_string(stack * s, char * str);

/* Given a pointer to a stack and a number of cells from the top */
/* cell, return an indexed cell without removing it */
extern void *       stack_read(stack * s, int offset);

/* Return the number of cells on a given stack */
extern int          stack_measure(stack * s);

/* Remove all cells from a given stack */
extern void         stack_clear(stack * s);

/* Free the backing store used by a stack, invalidating the stack */
extern void         stack_release(stack * s);

/* Some handy definitions when working with stacks of stack *'s */

#define stack_stack_top(t)            (stack *)stack_read(t, 0)
#define stack_stack_popcell(t)        stack_pop((stack *)stack_read(t, 0))
#define stack_stack_pushcell(t,s)     stack_push((stack *)stack_read(t, 0),(void *)s)

extern int stack_stack_transfer (stack * t, int count, void * zero);

#endif /* _F98STACK_H_ not defined */

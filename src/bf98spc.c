/*
   bf98spc.c v1.0 May 2011 Chris Pressey
   Dynamic Befunge-Space ANSI C Functions - source.

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
   - You need to store a large, dynamically allocated,
       Cartesian 2D space with 32-bit addressing and 32-bit cell data
   - You don't care about dynamic 1D, 3D etc spaces.
*/

#include <stdio.h>
#include <stdlib.h>

#include "bf98spc.h"

/* internal functions */

keytree * find_keytree(keytree * k, signed long int key_x, signed long int key_y, int * found)
{
  int compare = 0;
  keytree * l = k;

  *found = 0;

  if (l != NULL)
  {
    while (1)
    {
      compare = (l->key_x == key_x ? l->key_y - key_y : l->key_x - key_x);
      if (compare > 0)
      {
	if (l->right != NULL)
	{
	  l = l->right;
	} else
	{
	  return l;
	}
      } else
      if (compare < 0)
      {
	if (l->left != NULL)
	{
	  l = l->left;
	} else
	{
          /* This space intentionally left blank. */
	  return l;
	}
      } else
      {
	*found = 1;
	return l;
      }
    }
  } else
  {
    return NULL;
  }
  return l;
}

void destroy_keytree (keytree * k)
{
  if (k != NULL)
  {
    destroy_keytree (k->left);
    k->left = NULL;
    destroy_keytree (k->right);
    k->right = NULL;
    free(k->data);
    free(k);
  }
}

/* exported functions */

signed long int bfspace_fetch (bfspace * p, signed long int x, signed long int y)
{
  keytree * k;
  int found;

  k = find_keytree(p->root, (x >> SNIPPET_BITS), (y >> SNIPPET_BITS), &found);
  if (found)
  {
    if (k != NULL)
    {
      x = (x & SNIPPET_MASK);
      y = (y & SNIPPET_MASK);
      return k->data->space[(x << SNIPPET_BITS) + y];
    } else
    {
      return 32;
    }
  } else
  {
    /* Hello, sailor! */
    return 32;
  }
}

int bfspace_store (bfspace * p, signed long int x, signed long int y, signed long int data)
{
  keytree * n;
  keytree * k;
  int found, i;

  n = (keytree *)malloc(sizeof(keytree));
  n->left = NULL;
  n->right = NULL;
  n->key_x = x >> SNIPPET_BITS;
  n->key_y = y >> SNIPPET_BITS;

  k = p->root;
  if (k == NULL)
  {
    /* add as root. */
    p->root = n;
    n->data = (snippet *)malloc(sizeof(snippet));
    for (i = 0; i < (SNIPPET_SIZE*SNIPPET_SIZE); i++)
    {
      n->data->space[i] = 32;
    }
    if (x > p->max_x) p->max_x = x;
    if (x < p->min_x) p->min_x = x;
    if (y > p->max_y) p->max_y = y;
    if (y < p->min_y) p->min_y = y;
    x = (x & SNIPPET_MASK);
    y = (y & SNIPPET_MASK);
    n->data->space[(short)(x << SNIPPET_BITS) + (short)(y)] = data;
    return 1;
  } else
  {
    k = find_keytree(k, n->key_x, n->key_y, &found);
    if (found)
    {
      if (k != NULL)
      {
	if (x > p->max_x) p->max_x = x;
	if (x < p->min_x) p->min_x = x;
	if (y > p->max_y) p->max_y = y;
	if (y < p->min_y) p->min_y = y;
	x = (x & SNIPPET_MASK);
	y = (y & SNIPPET_MASK);
	k->data->space[(short)(x << SNIPPET_BITS) + (short)(y)] = data;
	free (n);
	return 1;
      } else
      {
	return 0;
      }
    } else
    {
      if (((k->key_x == n->key_x ? k->key_y - n->key_y : k->key_x - n->key_x) < 0) && (k->left == NULL))
      {
	k->left = n;
      } else
      if (((k->key_x == n->key_x ? k->key_y - n->key_y : k->key_x - n->key_x) > 0) && (k->right == NULL))
      {
	k->right = n;
      } else
      {
	/* we'd be really, really screwed, but luckily this never seems to happen */
	return 0;
      }
      n->data = (snippet *)malloc(sizeof(snippet));
      for (i = 0; i < (SNIPPET_SIZE*SNIPPET_SIZE); i++)
      {
	n->data->space[i] = 32;
      }
      if (x > p->max_x) p->max_x = x;
      if (x < p->min_x) p->min_x = x;
      if (y > p->max_y) p->max_y = y;
      if (y < p->min_y) p->min_y = y;
      x = (x & SNIPPET_MASK);
      y = (y & SNIPPET_MASK);
      n->data->space[(short)(x << SNIPPET_BITS) + (short)(y)] = data;
      return 1;
    }
  }
}

int bfspace_inbounds (bfspace * p, signed long int x, signed long int y)
{
  /* "You can't tell a squid." -- Old Canadian saying */
  return ((x <= p->max_x) && (x >= p->min_x) &&
	  (y <= p->max_y) && (y >= p->min_y));
}

/* might as well pass a NULL for p, nothing is done with it */
bfspace * bfspace_alloc (bfspace * p)
{
  bfspace * n;
  n = (bfspace *)malloc(sizeof(bfspace));
  n->root = NULL;
  n->max_x = -MAXLONG;
  n->min_x = MAXLONG;
  n->max_y = -MAXLONG;
  n->min_y = MAXLONG;
  p = p;
  return n;
}

void bfspace_free (bfspace * p)
{
  destroy_keytree(p->root);
  free(p);
}

int bfspace_fread (bfspace * p, FILE * f,
                   signed long int x, signed long int y,
                   signed long int * w, signed long int * h,
                   unsigned long int flags)
{
  signed long int col = x;
  signed long int row = y;
  signed long int a = 0;

  /* Efficiency at any cost! */

  *w = 0;  *h = 0;

  while (!feof (f))
  {
    a = (long) 0 | fgetc(f);
    if ( ((a == (long)0x0a) || (a == (long)0x0d)) &&
         ((flags & (long)0x01) != (long)0x01) )
    {
      if (a == (long)'\x0d')
      {
        /* next char might be \x0a, and if so, suck it up too. */
        a = (long) 0 | fgetc(f);
        if (a != (long)'\x0a') ungetc(a, f);
      }
      col = x;
      row++;  if ((row-y) > *h) *h = (row-y);
    } else
    {
      if (!feof(f))
      {
        if (a != ' ')
        {
	  if (!(bfspace_store(p, col++, row, a))) return 0;
          if ((col-x) > *w) *w = (col-x);
        } else
        {
          col++;
        }
      }
    }
  }
  (*h)++; /* bugfix 2003.0726 correct number of lines (= newlines + 1) */
  return 1;
}

int bfspace_fwrite (bfspace * p, FILE * f, signed long int x, signed long int y,
			signed long int w, signed long int h, unsigned long int flags)
{
  signed long int col = x;
  signed long int row = y;
  signed long int a = 0;

  signed long spccnt = 0;
  signed long i = 0;

  while ((row-y) < h)
  {
    a = bfspace_fetch(p, col++, row);
    spccnt=0;
    while (a == ((long)0|' ') && (col-x) <= w)
    {
      spccnt++;
      a = bfspace_fetch(p, col++, row);
    }
    /* begin bugfix 2003.0722: compressed vs non-compressed text output */
    if ((col-x) > w)                        /* if we are past the right edge */
    {
      if ((flags & (long)0x1) == (long)0x1) /* and we are compressing text */
      {
	/* print nothing */
      } else
      {
	/* print spaces only */
        for(i=0;i<spccnt;i++) fputc(' ', f);
      }
    } else
    {
      /* print spaces, then character */
      for(i=0;i<spccnt;i++) fputc(' ', f);
      fputc((char)a, f);
    }
    /* end bugfix 2003.0722 */
    if (col-x > w)
    {
      col = x;
      row++;
      fputc('\n', f);
    }
  }
  return 1;
}

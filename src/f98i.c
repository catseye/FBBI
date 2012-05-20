/*
   f98i.c for fbbi.c v1.0 May 2011 Chris Pressey
   Funge-98 Instructions - source.

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fbbi.h"
#include "bf98spc.h"
#include "f98ip.h"
#include "f98i.h"
#include "f98fp.h"

extern char ** environ;
extern char ** global_argv;
extern int global_argc;
extern int fungeprog_arg;

/*-- INSTRUCTIONS */

void fi_space  (ip * i) { i = i; /* THIS NEVER HAPPENS */ }
void fi_nop    (ip * i) { fi_space(i); }
void fi_reverse(ip * i) { NEG(i->dx); NEG(i->dy); }

void fi_die    (ip * i) { i->hm = 1; i->ec = 0; }
void fi_quit   (ip * i) { i->hm = 1; i->ec = ip_pop(i); }

void fi_digit  (ip * i) { ip_push(i, i->ir - '0'); }
void fi_hexdig (ip * i) { ip_push(i, i->ir - 'a' + 10); }

void fi_north  (ip * i) { DELTA(i,0,-1); }
void fi_south  (ip * i) { DELTA(i,0,1); }
void fi_east   (ip * i) { DELTA(i,1,0); }
void fi_west   (ip * i) { DELTA(i,-1,0); }

void fi_vertif (ip * i)
{
  i->dx = 0;
  if (ip_pop(i) != 0)
    i->dy = -1; else
    i->dy = 1;
}
void fi_horzif (ip * i)
{
  i->dy = 0;
  if (ip_pop(i) != 0)
    i->dx = -1; else
    i->dx = 1;
}
void fi_add    (ip * i) { ip_push(i, ip_pop(i) + ip_pop(i)); }
void fi_sub    (ip * i) { GET_ab(i); ip_push(i, i->b - i->a); }
void fi_mul    (ip * i) { ip_push(i, ip_pop(i) * ip_pop(i)); }
void fi_div    (ip * i) { GET_ab(i); if (i->a != 0) ip_push(i, i->b / i->a); else ip_push(i, 0); }
void fi_mod    (ip * i) { GET_ab(i); if (i->a != 0) ip_push(i, i->b % i->a); else ip_push(i, 0); }
void fi_swap   (ip * i) { GET_ab(i); ip_push(i, i->a); ip_push(i, i->b); }
void fi_string (ip * i) { i->sm = !i->sm; }
void fi_odec   (ip * i)
{
  i->b = 0;
  #ifndef FBBI_MINIMAL
  if (trace)
    i->b = printf ("***** Decimal: %ld\n", ip_pop(i)); else
  #endif
    i->b = printf ("%ld ", ip_pop(i));
  fflush (stdout);
  if (i->b == EOF) ip_reverse(i);
}

void fi_ochar  (ip * i)
{
  GET_a(i);
  i->b = 0;
  #ifndef FBBI_MINIMAL
  if (trace)
    i->b = printf ("***** Character: 0x%08lx ('%c')\n", i->a, PRINT(i->a)); else
  #endif
    i->b = printf ("%c", (char)i->a);
  fflush (stdout);
  if (i->b == EOF) ip_reverse(i);
}

void fi_dup    (ip * i) { GET_a(i); ip_push(i, i->a); ip_push(i, i->a);  }
void fi_not    (ip * i) { if (ip_pop(i) != 0) ip_push(i, 0); else ip_push(i, 1); }
void fi_greater(ip * i) { GET_a(i); if (ip_pop(i) > i->a) ip_push(i, 1); else ip_push(i, 0); }
void fi_ramp   (ip * i) { ip_march(i); }
void fi_char   (ip * i) { ip_march(i); ip_push(i, bfspace_fetch(i->bs, i->x, i->y)); }
void fi_store  (ip * i) { ip_march(i); bfspace_store(i->bs, i->x, i->y, ip_pop(i)); }
void fi_pop    (ip * i) { GET_a(i); }
void fi_away   (ip * i)
{
  DELTA(i,0,1);
  switch ((rand() / 32) % 4)
  {
    case 0: DELTA(i,1,0); break;
    case 1: DELTA(i,-1,0); break;
    case 2: i->dy=-1;
  }
}

void fi_idec   (ip * i)
{
  fflush(stdin);
  do
  {
    i->a = (long)0|fgetc(stdin);
    if ((i->a >= '0') && (i->a <= '9')) ungetc(i->a, stdin);
  } while ((i->a < '0') || (i->a > '9'));

  if(scanf ("%ld", &i->a) != EOF)
  {
    ip_push(i, i->a);
  } else
  {
    ip_reverse(i);
  }
}

void fi_ichar  (ip * i)
{
  fflush(stdin);
  i->a = (long)0 | fgetc(stdin);
  if (i->a != ((long)0 | EOF))
  {
    ip_push(i, i->a);
  } else
  {
    ip_reverse(i);
  }
}

void fi_get    (ip * i) { GET_a(i); ip_push(i, bfspace_fetch(i->bs, ip_pop(i)+i->sx, i->a+i->sy)); }
void fi_put    (ip * i) { GET_ab(i); bfspace_store(i->bs, i->b+i->sx, i->a+i->sy, ip_pop(i)); }

void fi_skip   (ip * i) { i = i; /* THIS NEVER HAPPENS */ }

void fi_rleft  (ip * i) { i->a = i->dy; i->dy = -1 * i->dx; i->dx = i->a; }
void fi_rright (ip * i) { i->a = i->dx; i->dx = -1 * i->dy; i->dy = i->a; }
void fi_jump   (ip * i) { GET_a(i); i->x += i->dx * i->a; i->y += i->dy * i->a; }
void fi_clear  (ip * i) { ip_clearstack(i); }
void fi_compare(ip * i)
{
  GET_ab(i);
  if (i->a > i->b)
     fi_rleft(i); else
    if (i->a < i->b)
      fi_rright(i);
}
void fi_delta  (ip * i) { i->dy = ip_pop(i); i->dx = ip_pop(i); }
void fi_under  (ip * i)
{
  cell f;
  GET_a(i);
  if (ip_stacks_measure(i) <= 1)
  {
    ip_reverse(i);
  } else if (i->a > 0)
  {
    for (i->b = 0; i-> b < i->a; i->b++)
    {
      f = ip_pop_soss(i);
      ip_push(i, f);
    }
  } else if (i->a < 0)
  {
    for (i->b=0;i->b > i->a;i->b--)
    {
      ip_push_soss(i, ip_pop(i));
    }
  }
}
void fi_begin (ip * i)
{
  GET_a(i);
  /* Regardless of sign of popped value, push new stack on SS */
  if (!ip_allocstack(i))
  {
    ip_reverse(i);
    return;
  }
  if (i->a<0)
  {
    /* Negative argument to { pushes |n| 0s onto the SOSS */
    while(i->a < 0)
    {
      ip_push_soss(i, 0);
      i->a++;
    }
  } else if (i->a > 0) ip_transfer(i, i->a);
  ip_push_soss(i, i->sx);
  ip_push_soss(i, i->sy);
  i->sx = i->x + i->dx;
  i->sy = i->y + i->dy;
  /*printf("$$$ EXECUTING 'BEGIN': assigned storage offset %ld, %ld\n", i->sx, i->sy);*/
}

void fi_end   (ip * i)
{
  /* check if there *is* a SOSS very first */
  if (ip_stacks_measure(i) <= 1)
  {
    ip_reverse(i);
    return;
  }
  GET_a(i);
  if (i->a<0)
  {
    /* Negative argument to } destroys top |n| values on the SOSS. */
    if(ip_stacks_measure(i) > 1)
    {
      while(i->a < 0)
      {
        ip_pop_soss(i);
        i->a++;
      }
    }
  }
  /* bugfix 2003.0722: } should still attempt to pop the TOSS */
  i->sy = ip_pop_soss(i);
  i->sx = ip_pop_soss(i);
  if (i->a > 0) ip_transfer(i, -1 * i->a);
  ip_freestack(i);
  /*printf("$$$ EXECUTING 'END': restored storage offset %ld, %ld\n", i->sx, i->sy);*/
}

void fi_in    (ip * i)
{
  cell c, d, flags;
  FILE *f; char s[256];

  ip_pop_string(i, s);
  flags = ip_pop(i);
  GET_ab(i);

  if((f=fopen(s, "r"))!=NULL)
  {
    bfspace_fread(i->bs, f, i->b+i->sx, i->a+i->sy, &c, &d, flags);
    fclose(f);
    ip_push(i, c);
    ip_push(i, d);
    ip_push(i, i->b);
    ip_push(i, i->a);
  } else ip_reverse(i);
}

void fi_out   (ip * i)
{
  FILE *f; char s[256];
  cell zx, zy, flags;
  ip_pop_string(i, s);
  flags = ip_pop(i);
  GET_ab(i);

  zy = ip_pop(i);
  zx = ip_pop(i);

  if((f=fopen(s, "w"))!=NULL)
  {
    bfspace_fwrite(i->bs, f, i->b+i->sx, i->a+i->sy,
                   zx, zy, flags);
    fclose(f);
  } else ip_reverse(i);
}
void fi_exec  (ip * i)
{
  char s[256];
  ip_pop_string(i, s);
  ip_push(i, system(s));
}
void fi_unimp (ip * i)
{
  #ifndef FBBI_MINIMAL
  if(warn)
    fprintf(stderr, "fbbi warning: unimplemented 0x%08lx at (%ld,%ld)\n",
                    i->ir, i->x, i->y);
  #endif
  fi_reverse(i);
}
void fi_nonb93 (ip * i)
{
  #ifndef FBBI_MINIMAL
  if(warn)
    fprintf(stderr, "fbbi warning: non-Befunge-93 instruction 0x%08lx at (%ld,%ld)\n",
                    i->ir, i->x, i->y);
  #endif
  /* fi_reverse(i); not in b93 compat mode! */
}


void fi_iterate(ip * i)
{
  cell c, sx, sy;
  c = ip_pop(i);
  sx = i->x;
  sy = i->y;
  ip_move(i);
  i->ir = bfspace_fetch(i->bs, i->x, i->y);
  if (c <= 0)
    return;
  i->x = sx;
  i->y = sy;
  if ((i->ir < 32) || (i->ir > 126))
  {
    fi_unimp(i);
    return;
  }
  for(;c;c--)
    instable[(char)i->ir-32](i);
}

void fi_pushsem(ip * i)
{
  i->a = get_fingerprint(i);
#ifndef FBBI_MINIMAL
  if (fingerprint_semantics(i, i->a, 1))
  {
    ip_push(i, i->a);
    ip_push(i, 1);
  } else
#endif
  fi_reverse(i);
}

void fi_popsem(ip * i)
{
  i->a = get_fingerprint(i);
#ifndef FBBI_MINIMAL
  if (!fingerprint_semantics(i, i->a, 0))
#endif
    fi_reverse(i);
}

void fi_sysinfo(ip * i)
{
  time_t t;
  struct tm * lt;
  cell sscs;

  t = time(NULL);
  lt = localtime(&t);
  GET_a(i);
  sscs = ip_stack_measure(i);

  ip_push(i, 0);              /* environment double */
  ip_push(i, 0);              /* terminator */
  /*
  for(i->b=0;environ[i->b]!=NULL&&strlen(environ[i->b])>0;i->b++)
    ip_push_string(i, environ[i->b]);
  */

  ip_push(i, 0);              /* arguments double */
  ip_push(i, 0);              /* terminator */
  for(i->b=global_argc-1;i->b>=fungeprog_arg;i->b--)
    ip_push_string(i, global_argv[i->b]);

  for(i->b=-1 * ip_stacks_measure(i) + 1;i->b < 0;i->b++)        /* size of each stack under current */
     ip_push(i, ip_stack_measure_offset(i, i->b));
  ip_push(i, sscs);
  ip_push(i, ip_stacks_measure(i));    /* size of stack stack */
  ip_push(i,
        ((long)lt->tm_hour << 16)
      | ((long)lt->tm_min << 8)
      | ((long)lt->tm_sec)); /* hour/min/sec */
  ip_push(i,
        ((long)lt->tm_year << 16)
      | ((long)lt->tm_mon << 8)
      | ((long)lt->tm_mday)); /* year/month/day */

  ip_push(i, i->bs->max_x - i->bs->min_x);  /* largest */
  ip_push(i, i->bs->max_y - i->bs->min_y);

  ip_push(i, i->bs->min_x);                 /* smallest */
  ip_push(i, i->bs->min_y);

  ip_push(i, i->sx);            /* offset */
  ip_push(i, i->sy);
  ip_push(i, i->dx);            /* delta */
  ip_push(i, i->dy);
  ip_push(i, i->x);             /* position */
  ip_push(i, i->y);
  ip_push(i, 42);            /* unique Team ID */
  ip_push(i, 42);            /* unique IP ID */
  ip_push(i, 2);             /* scalars per vector */
#ifdef __MSDOS__
  ip_push(i, (long)0|'\\');  /* MS-DOS paradigm */
#else
  ip_push(i, (long)0|'/');   /* probably.  Mac users will have 2 suffer 4 now :-> */
#endif
  ip_push(i, 1);             /* C system() paradigm */
  ip_push(i, FBBI_VERSION_MAJOR * 100 + FBBI_VERSION_MINOR);
  ip_push(i, 0x46424249);    /* handprint */
  ip_push(i, sizeof(cell));  /* bytes per cell */
  ip_push(i, 0x0000000e);    /* flags */

  if (i->a > 0)
  {
    i->a = ip_stack_peek(i, i->a-1);
    while (ip_stack_measure(i) > sscs) ip_pop(i);
    ip_push(i, i->a);
  }
}

/*-- INSTRUCTION TABLE */

void (*instable[96])(ip *) =
{
  /* 32-63 */
  fi_space,  fi_not,    fi_string, fi_ramp,   fi_pop,    fi_mod,    fi_idec,   fi_char,
  fi_pushsem,fi_popsem, fi_mul,    fi_add,    fi_ochar,  fi_sub,    fi_odec,   fi_div,
  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,
  fi_digit,  fi_digit,  fi_dup,    fi_skip,   fi_west,   fi_exec,   fi_east,   fi_away,

  /* 64-95 */
  fi_die,    fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,
  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,
  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,  fi_unimp,
  fi_unimp,  fi_unimp,  fi_unimp,  fi_rleft,  fi_swap,   fi_rright, fi_north,  fi_horzif,

  /* 96-127 */
  fi_greater,fi_hexdig, fi_hexdig, fi_hexdig, fi_hexdig, fi_hexdig, fi_hexdig, fi_get,
  fi_unimp,  fi_in,     fi_jump,   fi_iterate,fi_unimp,  fi_unimp,  fi_clear,  fi_out,
  fi_put,    fi_quit,   fi_reverse,fi_store,  fi_unimp,  fi_under,  fi_south,  fi_compare,
  fi_delta,  fi_sysinfo,fi_nop,    fi_begin,  fi_vertif, fi_end,    fi_ichar,  fi_unimp
};

void (*b93instable[96])(ip *) =
{
  /* 32-63 */
  fi_space,  fi_not,    fi_string, fi_ramp,   fi_pop,    fi_mod,    fi_idec,   fi_nonb93,
  fi_nonb93, fi_nonb93, fi_mul,    fi_add,    fi_ochar,  fi_sub,    fi_odec,   fi_div,
  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,  fi_digit,
  fi_digit,  fi_digit,  fi_dup,    fi_nonb93, fi_west,   fi_nonb93, fi_east,   fi_away,

  /* 64-95 */
  fi_die,    fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93,
  fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93,
  fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93,
  fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_swap,   fi_nonb93, fi_north,  fi_horzif,

  /* 96-127 */
  fi_greater,fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_get,
  fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93,
  fi_put,    fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_south,  fi_nonb93,
  fi_nonb93, fi_nonb93, fi_nonb93, fi_nonb93, fi_vertif, fi_nonb93, fi_ichar,  fi_nonb93
};

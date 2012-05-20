/*
   fbbi.c v1.0 May 2011 Chris Pressey
   Flaming Bovine Befunge-98 Interpreter in ANSI C - Main Unit

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
   FBBI project at catseye.tc: http://catseye.tc/projects/fbbi/

   Usage: fbbi [-F] [-f] [-w] [-t] [-s] [-u] [-93] [-mc n] [-ms n]
               sourcefile [funge-prog-args]

      -F: not fast: less speed, but allows the program to be
          interrupted on systems that only check for such an
          interruption during I/O.
      -f: fast (default): more speed, but on some systems the
          program execution cannot be interrupted.
      -w: warn: like Perl's -w switch.  If an unimplimented
          instruction is executed, the user is warned.
          Used in conjunction with -93 or -u, alerts user to
          behaviour not in the Befunge-93 or Unefunge idioms,
          as appropriate.
      -t: trace: display debug info & samples of source
          during execution, with interactive prompt
      -s: script: begin execution on the first line that does
          not begin with a comment (#) symbol
      -u: unefunge: disables Befunge and Trefunge-specific (>1D)
          instructions and uses single-scalar vectors
     -93: disables Funge-98 instructions, changes stringmode
          and in conjunction with -w tells the user about it
     -mc: maxcells: specify the maximum number of cells on each
          stack, default 1024.
     -ms: maxstacks: specify the maximum number of stacks on the
          stack stack, default 256.
      sourcefile: name of file to load and treat as a Befunge source
      funge-prog-args: made available to Befunge program through y

   -------------------------------------------------------------

   v1.0, May 2011, Chris Pressey.
          * applied v2003.0722 patch:
            - have o not output trailing spaces when passed flag
            - { still pushes a new stack on the stack stack
            - } still attempts to pop the top of stack stack
          * applied Jeffrey Lee's patch (thanks Jeffrey!):
            - correct number of lines read on file input
            - ensure fingerprint semantics are saved correctly
            - correct wrapping when backtracking after #, j, etc
            - correct usage of EOS as string terminator
          * applied FBBI Mark 3 patch:
            - guard against negative fingerprint ID lengths
            - only r once if trying to k an unimplemented instr
          * IP position restored to location of k before execution
            of iterated instruction
          * fixed greatest bound of Funge-space reported by y
          * fixed off-by-one error in bfspace_fwrite, where the
            rightmost column of characters was not being output
          * fixed wrapping logic w.r.t. ; at edge of space
          * typed ip->s2 and ip_stack() as stack *, removed casts
          * reworked the stack-stack allocation routines, fixing
            a crippling memory allocation bug (both stacks and
            stack stacks were being alloced/freed, even though
            stacks overlayed stack stacks)
          * refactored debugger interface, made input more robust
          * fixed how v2003.0722 fixed { and } in a better way
          * fixed BSD license verbiage (I am not a Regent)
          + 'fast' execution is now the default; -F turns it off
          + improved Makefile (made vars default, added strip target.)
          X pushing the environment variables doesn't work anymore.

   v0.98a, Mar 26 2003, Chris Pressey.
          * relicensed under BSD license
	  * made minor changes to Makefile and docs
	  * values.h is no longer included under FreeBSD
	  
   v0.98, Oct 1 1998, Chris Pressey.
          * Negative argument to { pushes |n| 0s onto the SOSS
          * Negative argument to } destroys top |n| values on the SOSS.
          * } reflects when stack-stack would be about to underflow
          * y instruction now pushes args and env as per spec
          - fixed greatest point bug in y's behaviour
          + f98stack.c module generalized for stacks of any type 
          + Added trace ViewStackPlayfieldMoveDeltaOffset commands.
          + Unefunge compatibility [-u] (not well tested though).

   v0.94, Sep 15 1998, Chris Pressey.  [Unreleased]
          * . and , reflect if stdout fails for whatever reason.
          * { reflects when no more stacks are available
          * & sucks and discards all leading non-digit characters
          - worked around EOL (CR, LF or CRLF) bug (binary mode.)
          + added _FILE_H_ define checking on all included headers.

   v0.93, Aug 3 1998, Chris Pressey.
          * y pushes vectors and path seperators
          * i and o both take a flags argument now
          * ~ and & instructions act like r on EOF in stdin
          * space and ; "instructions" are never executed
            (though they do have meaning in stringmode)
          * k instruction takes argument from space
          - fixed off-by-one error in stack_push_string
          - fixed ss_push_offset and ss_pop_offset
          - made stack_push, ss_push_stack safer
          - explicitly externs environ in main()
          - changed time() code in y to more idiomatic use
          - = instruction now really pushes return code onto stack
          - upped default stack and stack stack sizes
          + added a gcc Makefile (-ansi -pedantic now doesn't complain)
          + added config.h so end user can recompile with own options
          + minimal build now knows no fingerprints (more minimal)
          + added gnuesque --long-name command line options
            (doesn't accept equals signs like --max-cells=1024 yet)
          + added Befunge-93 compatibility mode (not thoroughly tested)

   v0.92, Jul 20 1998, Chris Pressey.
          Brought into line with the changes to spec, which were:
          * added k Iterate and s Store Character instructions
          * i and o both now pop the filename first
          * i pushes results appropriate for input to o
          * sizes of stacks now pushed before command line in y
          * environment is now pushed after command line in y
          * Null fingerprint is now fingerprint number 0x0
          * Negative args to { or } now treated as zeros
          Also fixed the following bugs:
          - CRLF sequence indicates single EOL in bfspace_read
          - now (almost) cleans up y after taking a non-zero argument
          - wrapping with ';' instruction uses Lahey-wrap correctly
          Also made minor fbbi-only improvements, namely:
          + added size & script switches & longnames to command line
          + prettied up the source; made ip a struct, etc
          + modularized; everything in it's own object file
          + added 'mini' build, shaves 3K off the executable

   v0.91, Jul 8 1998, Chris Pressey.
          Added fingerprint mechanism and NULL and ROMA fingerprints.

   v0.90, Jul 6-8 1998, Chris Pressey.
          Did some critical tying up relative to the new spec.
          Added command line, final instructions {i=you}().

   v0.84, Mar 24 1998, Chris Pressey.
	  Added the bulk of vanilla Funge-98 instructions, and file I/O
	  functions for big Befunge-Space.

   v0.83, Mar 1998, Chris Pressey.
	  Derived from ssbf93 v0.83.

   -------------------------------------------------------------

   Mycology conformance drive: as of this writing, Mycology reports
   the following BADs with FBBI.  Here is our rationale for them:

    BAD: u with a positive argument gives strange storage offset:
           expected (0,0)

   Instrumenting FBBI with print statements reveals that Mycology is
   doing this at one point:
     $$$ EXECUTING 'BEGIN': assigned storage offset 14, 77
     $$$ EXECUTING 'END': restored storage offset 14, 13
   ...but later expecting a storage offset of (0,0).  This is despite
   no BAD being produced up to this point.  I've written my own little
   test for storage offset and u, in the under.b98 example source,
   and FBBI looks like it behaves as expected when saving, transfer-
   ing, and restoring the storage offset.  I am therefore classifying
   this BAD as a problem with Mycology, until further information
   comes to light.

    BAD: form feed reflects

   If form feeds indicate "increment Z coordinate" in Trefunge-ish
   Befunge sources, then form feeds shouldn't be loaded with the i
   instruction.  OTOH, if there are FF characters in Funge-space,
   they should reflect.

    BAD: after spacing top-left corner, y should report least point
           as ( -2 -1 ), not ( -10 -10 )
    BAD: after spacing top-left corner, y should report greatest point
           as ( 182 910 ), not ( 190 919 )
    BAD: after spacing right edge, least point reported by y should
           remain ( -2 -1 )
    BAD: after spacing right edge, y should report greatest point
           as ( 181 910 ), not ( 190 919 )
    BAD: after spacing bottom edge, y should report greatest point
           as ( 181 909 ), not ( 190 919 )

   These are because FBBI does not shrink its idea of the bounds of
   Funge-space, when characters at the edge of Funge-space are erased.
   However, the spec defines these values strangely -- a bounding box
   between the least and greatest points which contain non-space cells
   might not contain all non-space cells in Funge-space, which does not
   coincide with the idea that these values are "useful to give to the
   o instruction to output the entire program source as a text file".
   Thus we suffice to be content that the values FBBI provides for this
   purpose are sufficient, if not optimal, for passing to o to output
   the program as a text file.

    Environment variables

   There are some indications that FBBI does not handle environment
   variables properly in the y instruction.  However, understandably,
   Mycology does not try to test this.  I have contemplated how much
   I care, and have concluded that I don't consider this a blocker
   for the release of version 1.0.

*/

/*-- #INCLUDE'S */

/*---- C libraries */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#ifndef FBBI_ANSI
 #ifdef __MSDOS__
  #include <io.h>
 #endif
#endif

/*---- FBBI modules */

#include "config.h"

#include "fbbi.h"
#include "bf98spc.h"
#include "f98ip.h"
#include "f98i.h"
#include "f98fp.h"

/*-- GLOBALS */

char ** global_argv;		/* set to argv in main() */
int global_argc;		/* set to argc in main() */
int fungeprog_arg;		/* set to funge prog's arg in main() */

#ifndef FBBI_MINIMAL
flag fast=1;
flag warn=0;
flag trace=0;
flag script=0;
flag b93=0;
flag une=0;

flag bpt=0;
cell bx=0;
cell by=0;
#endif

/*-- TRACE */

#ifndef FBBI_MINIMAL
int ask_for_two_cells(cell * x, cell *y)
{
  cell savex = *x;
  cell savey = *y;
  int ic;

  fflush(stdin); 
  fflush(stdout);
  ic = scanf("%ld %ld", x, y);
  if (ic < 2) {
    *x = savex;
    *y = savey;
    printf("Could not understand your numbers -- aborting.\n");
    ic = 0;
  }
  fflush(stdin); 
  fflush(stdout);
  return ic;
}

void perform_trace(ip * i)
{
  char c[9];
  cell s[4];
  int j=0; int k=0; cell l=0; cell tx, ty;
  int ic;

  c[0] = PRINT(bfspace_fetch(i->bs, i->x-1, i->y-1));
  c[1] = PRINT(bfspace_fetch(i->bs, i->x, i->y-1));
  c[2] = PRINT(bfspace_fetch(i->bs, i->x+1, i->y-1));
  c[3] = PRINT(bfspace_fetch(i->bs, i->x-1, i->y));
  c[4] = PRINT(bfspace_fetch(i->bs, i->x, i->y));
  c[5] = PRINT(bfspace_fetch(i->bs, i->x+1, i->y));
  c[6] = PRINT(bfspace_fetch(i->bs, i->x-1, i->y+1));
  c[7] = PRINT(bfspace_fetch(i->bs, i->x, i->y+1));
  c[8] = PRINT(bfspace_fetch(i->bs, i->x+1, i->y+1));

  for(k=0;k<=3;k++)
  {
    if (ip_stack_measure(i) > k)
      s[k] = ip_stack_peek_peek(i, 0, k); else
      s[k] = 0;
  }

  printf("+---+ position   : (%06ld, %06ld) stack size: %d cell(s) # of stacks: %d\n",
                              i->x,  i->y,              ip_stack_measure(i), ip_stacks_measure(i));
  printf("|%c%c%c| delta      : (%06ld, %06ld) 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n",
          c[0], c[1], c[2],     i->dx, i->dy,  s[0],   s[1],    s[2],    s[3]);
  printf("|%c%c%c| offset     : (%06ld, %06ld) decimal (%ld %ld %ld %ld) ascii (%c%c%c%c)\n",
          c[3], c[4], c[5],     i->sx, i->sy,           s[0],s[1],s[2],s[3], PRINT(s[0]), PRINT(s[1]), PRINT(s[2]), PRINT(s[3]));
  printf("|%c%c%c| instruction: 0x%08lx ('%c')\n", c[6], c[7], c[8], i->ir, PRINT(i->ir));
  printf("+---+ stringmode : %s\n", i->sm ? "on" : "off");

  k=0;
  while(!k)
  {
    printf("S)tep R)un B)reakpoint V)iewStack P)layfield M)ove D)elta O)ffset Q)uit: ");
    fflush(stdin); 
    fflush(stdout);
    ic = scanf("%s", c);
    if (ic < 1)
    {
      c[0] = ' ';
      c[1] = (char)0;
    }
    fflush(stdin);
    fflush(stdout);
    if ((c[0] == 'r') || (c[0] == 'R')) { trace=0; k=1; }
    if ((c[0] == 'q') || (c[0] == 'Q')) { i->hm=1; k=1; }
    if ((c[0] == 's') || (c[0] == 'S')) { k=1; }
    if ((c[0] == 'v') || (c[0] == 'V'))
    {
      printf("%d stacks:\n", ip_stacks_measure(i));
      for(j=0;j<ip_stacks_measure(i);j++)
      {
        printf("  stack #%d, %d elements:\n",
                j + 1, ip_stack_measure_offset(i, j));
        for(k=0;k<ip_stack_measure_offset(i, j);k++)
        {
          l = ip_stack_peek_peek(i, j, k);
          printf("    element #%d = %ld ('%c').\n", k + 1, l, PRINT(l));
        }
      }
      k=0;
    }
    if ((c[0] == 'p') || (c[0] == 'P'))
    {
      printf("Enter x and y of upper-left, seperated by whitespace:");
      if (ask_for_two_cells(&tx, &ty))
      {
        bfspace_fwrite(i->bs, stdout, tx, ty, 79, 22, 0);
        fflush(stdout);
      }
      k=0;
    }
    if ((c[0] == 'm') || (c[0] == 'M'))
    {
      printf("Enter x and y coordinates seperated by whitespace:");
      ask_for_two_cells(&i->x, &i->y);
      k=0;
    }
    if ((c[0] == 'd') || (c[0] == 'D'))
    {
      printf("Enter dx and dy values seperated by whitespace:");
      ask_for_two_cells(&i->dx, &i->dy);
      k=0;
    }
    if ((c[0] == 'o') || (c[0] == 'O'))
    {
      printf("Enter x and y storage offsets seperated by whitespace:");
      ask_for_two_cells(&i->sx, &i->sy);
      k=0;
    }
    if ((c[0] == 'b') || (c[0] == 'B'))
    {
      if(!bpt)
      {
        printf("Enter x and y coordinates of breakpoint, seperated by whitespace:");
        if (ask_for_two_cells(&bx, &by))
          bpt=1;
      } else
      {
        printf("Breakpoint deactivated.");
        fflush(stdout);
        bpt=0;
      }
      k=0;
    }
  }
}
#endif

static void usage()
{
  printf("fbbi - flaming bovine befunge-98 interpreter v%d.%d",
          FBBI_VERSION_MAJOR, FBBI_VERSION_MINOR);
#ifdef FBBI_ANSI
  printf(" in ANSI C");
#endif
#ifdef FBBI_MINIMAL
  printf(" - mini");
#endif
  printf("\n    Copyright (c)1998-2011 Cat's Eye Technologies, http://catseye.tc/"
         "\n    Released under BSD license, see source code for more information.\n");
  printf("usage: fbbi ");
#ifndef FBBI_MINIMAL
  printf("[-F] [-f] [-w] [-t] [-s] [-u] [-93]\n            [-mc n] [-ms n] ");
#endif
  printf("sourcefile [funge-prog-args]\n");
#ifndef FBBI_MINIMAL
  printf(" -[not]F[ast]: go slower but allow program interrupt on some systems\n");
  printf(" -f[ast]: go faster but program cannot be interrupted on some systems\n");
  printf(" -w[arn]: issue warnings on unimplemented instructions\n");
  printf(" -t[race]: trace source execution during run\n");
  printf(" -s[cript]: begin execution on first non-# line of source\n");
  printf(" -u[nefunge]: work in one dimension only (Unefunge) mode\n");
  printf(" -[befunge]93: work in Befunge-93 compatibility mode\n");
  printf(" -m[ax]c[ells]: specify number of elements allocated for each stack, def. %d\n", DEFAULT_STACK_SIZE);
  printf(" -m[ax]s[tacks]: specify number of stacks allocated on stack stack, def. %d\n", DEFAULT_STACKSTACK_SIZE);
#endif
  exit(1);
}

/*-- MAIN PROGRAM */

int main(int argc, char **argv)
{
  int k, ec=0;
  FILE *f=NULL;
  bfspace *p=NULL;
  ip *i=NULL;
  cell w,h;
  int ssize  = DEFAULT_STACK_SIZE;
  int sssize = DEFAULT_STACKSTACK_SIZE;

  global_argv = argv;
  global_argc = argc;

  if (argc < 2)
    usage();

  fungeprog_arg = -1;
  for(k=1;k<argc;k++)
  {
#ifndef FBBI_MINIMAL
    if((!strcmp(argv[k], "-F")) || (!strcmp(argv[k], "-not-fast")) || (!strcmp(argv[k], "--not-fast"))) fast=0;
    if((!strcmp(argv[k], "-f")) || (!strcmp(argv[k], "-fast")) || (!strcmp(argv[k], "--fast"))) fast=1;
    if((!strcmp(argv[k], "-w")) || (!strcmp(argv[k], "-warn")) || (!strcmp(argv[k], "--warn"))) warn=1;
    if((!strcmp(argv[k], "-t")) || (!strcmp(argv[k], "-trace")) || (!strcmp(argv[k], "--trace"))) trace=1;
    if((!strcmp(argv[k], "-s")) || (!strcmp(argv[k], "-script")) || (!strcmp(argv[k], "--script"))) script = 1;
    if((!strcmp(argv[k], "-u")) || (!strcmp(argv[k], "-unefunge")) || (!strcmp(argv[k], "--unefunge"))) une = 1;
    if((!strcmp(argv[k], "-93")) || (!strcmp(argv[k], "-befunge93")) || (!strcmp(argv[k], "--befunge-93")) || (!strcmp(argv[k], "--befunge93"))) b93 = 1;
    if((!strcmp(argv[k], "-mc")) || (!strcmp(argv[k], "-maxcells")) || (!strcmp(argv[k], "--max-cells")) || (!strcmp(argv[k], "--maxcells")))
    {
      ssize = atoi(argv[++k]);
    } else
    if((!strcmp(argv[k], "-ms")) || (!strcmp(argv[k], "-maxstacks")) || (!strcmp(argv[k], "--max-stacks"))  || (!strcmp(argv[k], "--maxstacks")))
    {
      sssize = atoi(argv[++k]);
    } else
#endif
    if(argv[k][0] != '-') { fungeprog_arg = k; break; }
  }
  if (fungeprog_arg == -1)
    usage();

  /* initialize */

  srand(time(0));

  DEBUG("Allocating Funge-Space");
  if ((p = bfspace_alloc(NULL)) != NULL)
  {
    DEBUG("Allocating IP");
    if ((i = ip_alloc(p, ssize, sssize)) != NULL)
    {
      DEBUG("Opening Source File");
      if ((f=fopen(argv[fungeprog_arg],"r")) != NULL)
      {
#ifdef FBBI_MSDOS
 #ifndef FBBI_ANSI
        setmode(fileno(f), O_BINARY);
 #endif
#endif
        DEBUG("Reading Source File");
        if (bfspace_fread(i->bs, f, 0, 0, &w, &h, 0))
        {
          fclose(f);
#ifndef FBBI_MINIMAL
          if ((w > 79) || (h > 24))
          {
            if(b93 && warn)
              fprintf(stderr, "fbbi warning: "
              "source too large for Befunge-93-Space (%ld x %ld)\n", w+1, h+1);
          }
          if (h >= 1)
          {
            if(une && warn)
              fprintf(stderr, "fbbi warning: "
              "source too large for Unefunge-98-Space (%ld x %ld)\n", w+1, h+1);
          }
          if (script)
          {
            while (bfspace_fetch(i->bs, 0, i->y)
              == ((long)0 | (long)'#'))
              i->y++;
          }
#endif
          DEBUG("* Begin Interpret *");
          while (!i->hm)
          {
            DEBUG("Moving IP");
            ip_move(i);
#ifndef FBBI_MINIMAL
            if(bpt && i->x == bx && i->y == by) trace=1;
#endif
            DEBUG("Getting Instruction");
            i->ir = bfspace_fetch(i->bs, i->x, i->y);
#ifndef FBBI_MINIMAL
            if (b93 && ((i->x < 0) || (i->y < 0) || (i->x > 79) || (i->y > 24)) )
            {
              if(warn)
                fprintf(stderr, "fbbi warning: "
                "beyond Befunge-93-Space at (%ld,%ld)\n", i->x, i->y);
            }
            if (trace)
            {
              perform_trace(i);
              if (i->hm) break;
            }
#endif
            DEBUG("Executing Instruction");
            if (i->sm && ((char)i->ir != '"'))
            {
              ip_push(i, i->ir);
            } else
            {
              if ((i->ir < 32) || (i->ir > 126))
              {
                fi_unimp(i);
              } else
              {
#ifndef FBBI_MINIMAL
                if(b93) b93instable[(char)i->ir-32](i); else
#endif
                instable[(char)i->ir-32](i);
              }
            }
          }
          DEBUG("* End Interpret *");

          ec = (int)i->ec;
          DEBUG("Freeing IP");
          ip_free(i);
          DEBUG("Freeing Funge-Space");
          bfspace_free(p);

        } else
        {
          DEBUG("Freeing IP");
          ip_free(i);
          DEBUG("Freeing Funge-Space");
          bfspace_free(p);
          fclose(f);
#ifndef FBBI_MINIMAL
          fprintf(stderr, "fbbi error: can't load file\n");
#endif
          exit(1);
        }
      } else
      {
        DEBUG("Freeing IP");
        ip_free(i);
        DEBUG("Freeing Funge-Space");
        bfspace_free(p);
#ifndef FBBI_MINIMAL
        fprintf(stderr, "fbbi error: can't open file\n");
#endif
        exit(1);
      }
    } else
    {
      DEBUG("Freeing Funge-Space");
      bfspace_free(p);
#ifndef FBBI_MINIMAL
      fprintf(stderr, "fbbi error: can't allocate ip\n");
#endif
      exit(1);
    }
  } else
  {
#ifndef FBBI_MINIMAL
    fprintf(stderr, "fbbi error: can't allocate befunge-space\n");
#endif
    exit(1);
  }
  return ec;
}

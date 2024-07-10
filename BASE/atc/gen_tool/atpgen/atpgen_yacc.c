
/*  A Bison parser, made from atpgen.y
    by GNU Bison version 1.28  */

#define ATPBISON 1  /* Identify Bison output.  */

#define	ATC_DEF_NAME	257
#define	ATC_DEF_BEGIN	258
#define	ATC_DEF_END	259
#define	ATC_DEF_STRING	260
#define	ATC_DEF_NUMBER	261
#define	ATC_DEF_INFINITY	262

#include <stdio.h>
#include <stdlib.h>

#include "atp_atc_info.h"

typedef union { int                         val;
         ATC_PARAM_STRING_T          str_t;
         ATC_PARAM_NUMERIC_ARRAY_T * num_array_t;
         ATC_PARAM_STRING_ARRAY_T  * str_array_t;
         ATC_PARAM_INFO_T          * param_info_t;
         ATC_INFO_T                * info_t;
       } ATPSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	ATPFINAL		36
#define	ATPFLAG		-32768
#define	ATPNTBASE	14

#define ATPTRANSLATE(x) ((unsigned)(x) <= 262 ? atptranslate[x] : 24)

static const char atptranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    10,     2,     2,     2,     2,     2,    11,
    12,     2,     2,     9,    13,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8
};

#if ATPDEBUG != 0
static const short atpprhs[] = {     0,
     0,     2,     5,     7,    14,    18,    21,    25,    27,    31,
    35,    38,    42,    44,    47,    51,    53,    57,    59
};

static const short atprhs[] = {    15,
     0,    15,    16,     0,    16,     0,     3,     4,     7,     9,
    17,     5,     0,    10,    18,    10,     0,    10,    10,     0,
    18,     9,    19,     0,    19,     0,    11,    20,    12,     0,
    11,    22,    12,     0,    11,    12,     0,    20,     9,    21,
     0,    21,     0,     7,     7,     0,     7,    13,     7,     0,
     7,     0,    22,     9,    23,     0,    23,     0,     6,     0
};

#endif

#if ATPDEBUG != 0
static const short atprline[] = { 0,
    32,    39,    43,    49,    55,    59,    65,    69,    75,    79,
    83,    89,    93,    99,   104,   108,   114,   118,   124
};
#endif


#if ATPDEBUG != 0 || defined (ATPERROR_VERBOSE)

static const char * const atptname[] = {   "$","error","$undefined.","ATC_DEF_NAME",
"ATC_DEF_BEGIN","ATC_DEF_END","ATC_DEF_STRING","ATC_DEF_NUMBER","ATC_DEF_INFINITY",
"','","'\\\"'","'('","')'","'-'","atc_def_file","atc_defs","atc_def","atc_range",
"atc_range_params","atc_range_param","atc_range_numbers","atc_range_number",
"atc_range_strings","atc_range_string", NULL
};
#endif

static const short atpr1[] = {     0,
    14,    15,    15,    16,    17,    17,    18,    18,    19,    19,
    19,    20,    20,    21,    21,    21,    22,    22,    23
};

static const short atpr2[] = {     0,
     1,     2,     1,     6,     3,     2,     3,     1,     3,     3,
     2,     3,     1,     2,     3,     1,     3,     1,     1
};

static const short atpdefact[] = {     0,
     0,     1,     3,     0,     2,     0,     0,     0,     0,     6,
     0,     0,     8,     4,    19,    16,    11,     0,    13,     0,
    18,     0,     5,    14,     0,     0,     9,     0,    10,     7,
    15,    12,    17,     0,     0,     0
};

static const short atpdefgoto[] = {    34,
     2,     3,     9,    12,    13,    18,    19,    20,    21
};

static const short atppact[] = {     2,
     7,     2,-32768,     8,-32768,     9,     6,    -7,    12,-32768,
    -6,     4,-32768,-32768,-32768,    -5,-32768,    -2,-32768,     0,
-32768,    10,-32768,-32768,    13,    15,-32768,    17,-32768,-32768,
-32768,-32768,-32768,    19,    24,-32768
};

static const short atppgoto[] = {-32768,
-32768,    23,-32768,-32768,     5,-32768,     3,-32768,    11
};


#define	ATPLAST		39


static const short atptable[] = {    15,
    16,    24,    10,    11,     1,    17,    26,    25,    28,    27,
     4,    29,    22,    23,     6,     8,    14,     7,    35,    31,
    11,    16,    15,    36,     5,     0,    30,     0,    32,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    33
};

static const short atpcheck[] = {     6,
     7,     7,    10,    11,     3,    12,     9,    13,     9,    12,
     4,    12,     9,    10,     7,    10,     5,     9,     0,     7,
    11,     7,     6,     0,     2,    -1,    22,    -1,    26,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef ATPSTACK_USE_ALLOCA
#ifdef alloca
#define ATPSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define ATPSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define ATPSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define ATPSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define ATPSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* ATPSTACK_USE_ALLOCA not defined */

#ifdef ATPSTACK_USE_ALLOCA
#define ATPSTACK_ALLOC alloca
#else
#define ATPSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define atperrok		(atperrstatus = 0)
#define atpclearin	(atpchar = ATPEMPTY)
#define ATPEMPTY		-2
#define ATPEOF		0
#define ATPACCEPT	goto atpacceptlab
#define ATPABORT 	goto atpabortlab
#define ATPERROR		goto atperrlab1
/* Like ATPERROR except do call atperror.
   This remains here temporarily to ease the
   transition to the new meaning of ATPERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define ATPFAIL		goto atperrlab
#define ATPRECOVERING()  (!!atperrstatus)
#define ATPBACKUP(token, value) \
do								\
  if (atpchar == ATPEMPTY && atplen == 1)				\
    { atpchar = (token), atplval = (value);			\
      atpchar1 = ATPTRANSLATE (atpchar);				\
      ATPPOPSTACK;						\
      goto atpbackup;						\
    }								\
  else								\
    { atperror ("syntax error: cannot back up"); ATPERROR; }	\
while (0)

#define ATPTERROR	1
#define ATPERRCODE	256

#ifndef ATPPURE
#define ATPLEX		atplex()
#endif

#ifdef ATPPURE
#ifdef ATPLSP_NEEDED
#ifdef ATPLEX_PARAM
#define ATPLEX		atplex(&atplval, &atplloc, ATPLEX_PARAM)
#else
#define ATPLEX		atplex(&atplval, &atplloc)
#endif
#else /* not ATPLSP_NEEDED */
#ifdef ATPLEX_PARAM
#define ATPLEX		atplex(&atplval, ATPLEX_PARAM)
#else
#define ATPLEX		atplex(&atplval)
#endif
#endif /* not ATPLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef ATPPURE

int	atpchar;			/*  the lookahead symbol		*/
ATPSTYPE	atplval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef ATPLSP_NEEDED
ATPLTYPE atplloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int atpnerrs;			/*  number of parse errors so far       */
#endif  /* not ATPPURE */

#if ATPDEBUG != 0
int atpdebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  ATPINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	ATPINITDEPTH
#define ATPINITDEPTH 200
#endif

/*  ATPMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if ATPMAXDEPTH == 0
#undef ATPMAXDEPTH
#endif

#ifndef ATPMAXDEPTH
#define ATPMAXDEPTH 10000
#endif

/* Define __atp_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __atp_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__atp_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__atp_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define ATPPARSE_PARAM as the name of an argument to be passed
   into atpparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef ATPPARSE_PARAM
#ifdef __cplusplus
#define ATPPARSE_PARAM_ARG void *ATPPARSE_PARAM
#define ATPPARSE_PARAM_DECL
#else /* not __cplusplus */
#define ATPPARSE_PARAM_ARG ATPPARSE_PARAM
#define ATPPARSE_PARAM_DECL void *ATPPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not ATPPARSE_PARAM */
#define ATPPARSE_PARAM_ARG
#define ATPPARSE_PARAM_DECL
#endif /* not ATPPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef ATPPARSE_PARAM
int atpparse (void *);
#else
int atpparse (void);
#endif
#endif

int
atpparse(ATPPARSE_PARAM_ARG)
     ATPPARSE_PARAM_DECL
{
  register int atpstate;
  register int atpn;
  register short *atpssp;
  register ATPSTYPE *atpvsp;
  int atperrstatus;	/*  number of tokens to shift before error messages enabled */
  int atpchar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	atpssa[ATPINITDEPTH];	/*  the state stack			*/
  ATPSTYPE atpvsa[ATPINITDEPTH];	/*  the semantic value stack		*/

  short *atpss = atpssa;		/*  refer to the stacks thru separate pointers */
  ATPSTYPE *atpvs = atpvsa;	/*  to allow atpoverflow to reallocate them elsewhere */

#ifdef ATPLSP_NEEDED
  ATPLTYPE atplsa[ATPINITDEPTH];	/*  the location stack			*/
  ATPLTYPE *atpls = atplsa;
  ATPLTYPE *atplsp;

#define ATPPOPSTACK   (atpvsp--, atpssp--, atplsp--)
#else
#define ATPPOPSTACK   (atpvsp--, atpssp--)
#endif

  int atpstacksize = ATPINITDEPTH;
  int atpfree_stacks = 0;

#ifdef ATPPURE
  int atpchar;
  ATPSTYPE atplval;
  int atpnerrs;
#ifdef ATPLSP_NEEDED
  ATPLTYPE atplloc;
#endif
#endif

  ATPSTYPE atpval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int atplen;

#if ATPDEBUG != 0
  if (atpdebug)
    fprintf(stderr, "Starting parse\n");
#endif

  atpstate = 0;
  atperrstatus = 0;
  atpnerrs = 0;
  atpchar = ATPEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  atpssp = atpss - 1;
  atpvsp = atpvs;
#ifdef ATPLSP_NEEDED
  atplsp = atpls;
#endif

/* Push a new state, which is found in  atpstate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
atpnewstate:

  *++atpssp = atpstate;

  if (atpssp >= atpss + atpstacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      ATPSTYPE *atpvs1 = atpvs;
      short *atpss1 = atpss;
#ifdef ATPLSP_NEEDED
      ATPLTYPE *atpls1 = atpls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = atpssp - atpss + 1;

#ifdef atpoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef ATPLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if atpoverflow is a macro.  */
      atpoverflow("parser stack overflow",
		 &atpss1, size * sizeof (*atpssp),
		 &atpvs1, size * sizeof (*atpvsp),
		 &atpls1, size * sizeof (*atplsp),
		 &atpstacksize);
#else
      atpoverflow("parser stack overflow",
		 &atpss1, size * sizeof (*atpssp),
		 &atpvs1, size * sizeof (*atpvsp),
		 &atpstacksize);
#endif

      atpss = atpss1; atpvs = atpvs1;
#ifdef ATPLSP_NEEDED
      atpls = atpls1;
#endif
#else /* no atpoverflow */
      /* Extend the stack our own way.  */
      if (atpstacksize >= ATPMAXDEPTH)
	{
	  atperror("parser stack overflow");
	  if (atpfree_stacks)
	    {
	      free (atpss);
	      free (atpvs);
#ifdef ATPLSP_NEEDED
	      free (atpls);
#endif
	    }
	  return 2;
	}
      atpstacksize *= 2;
      if (atpstacksize > ATPMAXDEPTH)
	atpstacksize = ATPMAXDEPTH;
#ifndef ATPSTACK_USE_ALLOCA
      atpfree_stacks = 1;
#endif
      atpss = (short *) ATPSTACK_ALLOC (atpstacksize * sizeof (*atpssp));
      __atp_memcpy ((char *)atpss, (char *)atpss1,
		   size * (unsigned int) sizeof (*atpssp));
      atpvs = (ATPSTYPE *) ATPSTACK_ALLOC (atpstacksize * sizeof (*atpvsp));
      __atp_memcpy ((char *)atpvs, (char *)atpvs1,
		   size * (unsigned int) sizeof (*atpvsp));
#ifdef ATPLSP_NEEDED
      atpls = (ATPLTYPE *) ATPSTACK_ALLOC (atpstacksize * sizeof (*atplsp));
      __atp_memcpy ((char *)atpls, (char *)atpls1,
		   size * (unsigned int) sizeof (*atplsp));
#endif
#endif /* no atpoverflow */

      atpssp = atpss + size - 1;
      atpvsp = atpvs + size - 1;
#ifdef ATPLSP_NEEDED
      atplsp = atpls + size - 1;
#endif

#if ATPDEBUG != 0
      if (atpdebug)
	fprintf(stderr, "Stack size increased to %d\n", atpstacksize);
#endif

      if (atpssp >= atpss + atpstacksize - 1)
	ATPABORT;
    }

#if ATPDEBUG != 0
  if (atpdebug)
    fprintf(stderr, "Entering state %d\n", atpstate);
#endif

  goto atpbackup;
 atpbackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* atpresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  atpn = atppact[atpstate];
  if (atpn == ATPFLAG)
    goto atpdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* atpchar is either ATPEMPTY or ATPEOF
     or a valid token in external form.  */

  if (atpchar == ATPEMPTY)
    {
#if ATPDEBUG != 0
      if (atpdebug)
	fprintf(stderr, "Reading a token: ");
#endif
      atpchar = ATPLEX;
    }

  /* Convert token to internal form (in atpchar1) for indexing tables with */

  if (atpchar <= 0)		/* This means end of input. */
    {
      atpchar1 = 0;
      atpchar = ATPEOF;		/* Don't call ATPLEX any more */

#if ATPDEBUG != 0
      if (atpdebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      atpchar1 = ATPTRANSLATE(atpchar);

#if ATPDEBUG != 0
      if (atpdebug)
	{
	  fprintf (stderr, "Next token is %d (%s", atpchar, atptname[atpchar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef ATPPRINT
	  ATPPRINT (stderr, atpchar, atplval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  atpn += atpchar1;
  if (atpn < 0 || atpn > ATPLAST || atpcheck[atpn] != atpchar1)
    goto atpdefault;

  atpn = atptable[atpn];

  /* atpn is what to do for this token type in this state.
     Negative => reduce, -atpn is rule number.
     Positive => shift, atpn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (atpn < 0)
    {
      if (atpn == ATPFLAG)
	goto atperrlab;
      atpn = -atpn;
      goto atpreduce;
    }
  else if (atpn == 0)
    goto atperrlab;

  if (atpn == ATPFINAL)
    ATPACCEPT;

  /* Shift the lookahead token.  */

#if ATPDEBUG != 0
  if (atpdebug)
    fprintf(stderr, "Shifting token %d (%s), ", atpchar, atptname[atpchar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (atpchar != ATPEOF)
    atpchar = ATPEMPTY;

  *++atpvsp = atplval;
#ifdef ATPLSP_NEEDED
  *++atplsp = atplloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (atperrstatus) atperrstatus--;

  atpstate = atpn;
  goto atpnewstate;

/* Do the default action for the current state.  */
atpdefault:

  atpn = atpdefact[atpstate];
  if (atpn == 0)
    goto atperrlab;

/* Do a reduction.  atpn is the number of a rule to reduce with.  */
atpreduce:
  atplen = atpr2[atpn];
  if (atplen > 0)
    atpval = atpvsp[1-atplen]; /* implement default value of the action */

#if ATPDEBUG != 0
  if (atpdebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       atpn, atprline[atpn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = atpprhs[atpn]; atprhs[i] > 0; i++)
	fprintf (stderr, "%s ", atptname[atprhs[i]]);
      fprintf (stderr, " -> %s\n", atptname[atpr1[atpn]]);
    }
#endif


  switch (atpn) {

case 1:
{
                        atp_store_all_defs(atpvsp[0].info_t);
                        ATPACCEPT;
                      ;
    break;}
case 2:
{
                        atpval.info_t = atp_link_info_defs(atpvsp[-1].info_t, atpvsp[0].info_t);
                      ;
    break;}
case 3:
{
                        atpval.info_t = atp_link_info_defs(atpvsp[0].info_t, NULL);
                      ;
    break;}
case 4:
{
                        atpval.info_t = atp_store_one_info_def(atpvsp[-5].str_t, atpvsp[-3].val, atpvsp[-1].param_info_t);
                      ;
    break;}
case 5:
{
                        atpval.param_info_t = atpvsp[-1].param_info_t;
                      ;
    break;}
case 6:
{
                        atpval.param_info_t = NULL;
                      ;
    break;}
case 7:
{
                        atpval.param_info_t = atp_link_param_ranges(atpvsp[-2].param_info_t, atpvsp[0].param_info_t);
                      ;
    break;}
case 8:
{
                        atpval.param_info_t = atp_link_param_ranges(atpvsp[0].param_info_t, NULL);
                      ;
    break;}
case 9:
{
                        atpval.param_info_t = atp_store_numeric_param_range(atpvsp[-1].num_array_t);
                      ;
    break;}
case 10:
{
                        atpval.param_info_t = atp_store_string_param_range(atpvsp[-1].str_array_t);
                      ;
    break;}
case 11:
{
                        atpval.param_info_t = atp_store_unknown_param_range();
                      ;
    break;}
case 12:
{
                        atpval.num_array_t = atp_link_numeric_param_ranges(atpvsp[-2].num_array_t, atpvsp[0].num_array_t);
                      ;
    break;}
case 13:
{
                        atpval.num_array_t = atp_link_numeric_param_ranges(atpvsp[0].num_array_t, NULL);
                      ;
    break;}
case 14:
{
                        if (atpvsp[0].val < 0)
                          atpval.num_array_t = atp_store_one_numeric_param_range(atpvsp[-1].val, -atpvsp[0].val);
                      ;
    break;}
case 15:
{
                        atpval.num_array_t = atp_store_one_numeric_param_range(atpvsp[-2].val, atpvsp[0].val);
                      ;
    break;}
case 16:
{
                        atpval.num_array_t = atp_store_one_numeric_param_range(atpvsp[0].val, atpvsp[0].val);
                      ;
    break;}
case 17:
{
                        atpval.str_array_t = atp_link_string_param_ranges(atpvsp[-2].str_array_t, atpvsp[0].str_array_t);
                      ;
    break;}
case 18:
{
                        atpval.str_array_t = atp_link_string_param_ranges(atpvsp[0].str_array_t, NULL);
                      ;
    break;}
case 19:
{
                        atpval.str_array_t = atp_store_one_string_param_range(atpvsp[0].str_t);
                      ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  atpvsp -= atplen;
  atpssp -= atplen;
#ifdef ATPLSP_NEEDED
  atplsp -= atplen;
#endif

#if ATPDEBUG != 0
  if (atpdebug)
    {
      short *ssp1 = atpss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != atpssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++atpvsp = atpval;

#ifdef ATPLSP_NEEDED
  atplsp++;
  if (atplen == 0)
    {
      atplsp->first_line = atplloc.first_line;
      atplsp->first_column = atplloc.first_column;
      atplsp->last_line = (atplsp-1)->last_line;
      atplsp->last_column = (atplsp-1)->last_column;
      atplsp->text = 0;
    }
  else
    {
      atplsp->last_line = (atplsp+atplen-1)->last_line;
      atplsp->last_column = (atplsp+atplen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  atpn = atpr1[atpn];

  atpstate = atppgoto[atpn - ATPNTBASE] + *atpssp;
  if (atpstate >= 0 && atpstate <= ATPLAST && atpcheck[atpstate] == *atpssp)
    atpstate = atptable[atpstate];
  else
    atpstate = atpdefgoto[atpn - ATPNTBASE];

  goto atpnewstate;

atperrlab:   /* here on detecting error */

  if (! atperrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++atpnerrs;

#ifdef ATPERROR_VERBOSE
      atpn = atppact[atpstate];

      if (atpn > ATPFLAG && atpn < ATPLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -atpn if nec to avoid negative indexes in atpcheck.  */
	  for (x = (atpn < 0 ? -atpn : 0);
	       x < (sizeof(atptname) / sizeof(char *)); x++)
	    if (atpcheck[x + atpn] == x)
	      size += strlen(atptname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (atpn < 0 ? -atpn : 0);
		       x < (sizeof(atptname) / sizeof(char *)); x++)
		    if (atpcheck[x + atpn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, atptname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      atperror(msg);
	      free(msg);
	    }
	  else
	    atperror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* ATPERROR_VERBOSE */
	atperror("parse error");
    }

  goto atperrlab1;
atperrlab1:   /* here on error raised explicitly by an action */

  if (atperrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (atpchar == ATPEOF)
	ATPABORT;

#if ATPDEBUG != 0
      if (atpdebug)
	fprintf(stderr, "Discarding token %d (%s).\n", atpchar, atptname[atpchar1]);
#endif

      atpchar = ATPEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  atperrstatus = 3;		/* Each real token shifted decrements this */

  goto atperrhandle;

atperrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  atpn = atpdefact[atpstate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (atpn) goto atpdefault;
#endif

atperrpop:   /* pop the current state because it cannot handle the error token */

  if (atpssp == atpss) ATPABORT;
  atpvsp--;
  atpstate = *--atpssp;
#ifdef ATPLSP_NEEDED
  atplsp--;
#endif

#if ATPDEBUG != 0
  if (atpdebug)
    {
      short *ssp1 = atpss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != atpssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

atperrhandle:

  atpn = atppact[atpstate];
  if (atpn == ATPFLAG)
    goto atperrdefault;

  atpn += ATPTERROR;
  if (atpn < 0 || atpn > ATPLAST || atpcheck[atpn] != ATPTERROR)
    goto atperrdefault;

  atpn = atptable[atpn];
  if (atpn < 0)
    {
      if (atpn == ATPFLAG)
	goto atperrpop;
      atpn = -atpn;
      goto atpreduce;
    }
  else if (atpn == 0)
    goto atperrpop;

  if (atpn == ATPFINAL)
    ATPACCEPT;

#if ATPDEBUG != 0
  if (atpdebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++atpvsp = atplval;
#ifdef ATPLSP_NEEDED
  *++atplsp = atplloc;
#endif

  atpstate = atpn;
  goto atpnewstate;

 atpacceptlab:
  /* ATPACCEPT comes here.  */
  if (atpfree_stacks)
    {
      free (atpss);
      free (atpvs);
#ifdef ATPLSP_NEEDED
      free (atpls);
#endif
    }
  return 0;

 atpabortlab:
  /* ATPABORT comes here.  */
  if (atpfree_stacks)
    {
      free (atpss);
      free (atpvs);
#ifdef ATPLSP_NEEDED
      free (atpls);
#endif
    }
  return 1;
}

int atperror( char * s, ... )
{
    printf("%s", s);
    return 0;
}

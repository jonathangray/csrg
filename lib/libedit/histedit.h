/*-
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Christos Zoulas of Cornell University.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)histedit.h	5.1 (Berkeley) 06/22/92
 */

/*
 * editline.h: Line editor interface
 */
#ifndef _h_editline
#define _h_editline

#include <sys/types.h>
#include <stdio.h>

typedef struct editline EditLine;

/*
 * For user-defined function interface
 */
typedef struct lineinfo {
    const char *buffer;
    const char *cursor;
    const char *lastchar;
} LineInfo;

/*
 * el_set/el_get parameters
 */
#define EL_PROMPT	0
#define EL_FILE		1
#define EL_TERMINAL	2
#define EL_EDITOR	3
#define EL_SIGNAL	4
#define	EL_BIND		5
#define	EL_TELLTC	6
#define	EL_SETTC	7
#define	EL_ECHOTC	8
#define	EL_SETTY	9
#define	EL_ADDFN	10
#define EL_HIST		11

/*
 * EditLine editor function return codes.
 * For user-defined function interface
 */
#define	CC_NORM		0
#define	CC_NEWLINE	1
#define	CC_EOF		2
#define CC_ARGHACK	3
#define CC_REFRESH	4
#define	CC_CURSOR	5
#define	CC_ERROR	6
#define CC_FATAL	7

/*
 * Initialization, cleanup, and resetting
 */
EditLine	*el_init	__P((const char *, FILE *, FILE *));
void		 el_reset	__P((EditLine *));
void		 el_end		__P((EditLine *));


/*
 * Get a line, a character or push a string back in the input queue
 */
const char      *el_gets	__P((EditLine *, int *));
int		 el_getc	__P((EditLine *, char *));
void		 el_push	__P((EditLine *, const char *));

/*
 * High level function internals control
 * Parses argc, argv array and executes builtin editline commands
 */
int		 el_parse	__P((EditLine *, int, char **)); 

/*
 * Low level editline access function
 */
int 		 el_set		__P((EditLine *, int, ...));

/*
 * Must be called when the terminal changes size; If EL_SIGNAL
 * is set this is done automatically otherwise it is the responsibility
 * of the application
 */
void		 el_resize	__P((EditLine *));


/*
 * User-defined function interface.
 */
const LineInfo	*el_line	__P((EditLine *));
int   		 el_insertstr	__P((EditLine *, char *));
void		 el_deletestr	__P((EditLine *, int));

#endif /* _h_editline */

/*-
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
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
 *	@(#)operators.h	5.1 (Berkeley) 04/30/93
 */

#define	NOT		0
#define	ISBLOCK		1
#define	ISCHAR		2
#define	ISDIR		3
#define	ISEXIST		4
#define	ISFILE		5
#define	ISSETGID	6
#define	ISSTICKY	7
#define	STRLEN		8
#define	ISFIFO		9
#define	ISREAD		10
#define	ISSIZE		11
#define	ISTTY		12
#define	ISSETUID	13
#define	ISWRITE		14
#define	ISEXEC		15
#define	NULSTR		16

#define	FIRST_BINARY_OP	17
#define	OR1		17
#define	OR2		18
#define	AND1		19
#define	AND2		20
#define	STREQ		21
#define	STRNE		22
#define	EQ		23
#define	NE		24
#define	GT		25
#define	LT		26
#define	LE		27
#define	GE		28


#define	OP_INT		1	/* arguments to operator are integer */
#define	OP_STRING	2	/* arguments to operator are string */
#define	OP_FILE		3	/* argument is a file name */

extern char *const unary_op[];
extern char *const binary_op[];
extern const char op_priority[];
extern const char op_argflag[];

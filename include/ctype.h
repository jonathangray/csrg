/*
 * Copyright (c) 1989 The Regents of the University of California.
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
 *	@(#)ctype.h	5.7 (Berkeley) 03/13/92
 */

#ifndef _CTYPE_H_
#define _CTYPE_H_

#define	_U	0x01
#define	_L	0x02
#define	_N	0x04
#define	_S	0x08
#define	_P	0x10
#define	_C	0x20
#define	_X	0x40
#define	_B	0x80

extern char __ctype[], __maplower[], __mapupper[];

#define	isalnum(c)	((__ctype + 1)[c] & (_U|_L|_N))
#define	isalpha(c)	((__ctype + 1)[c] & (_U|_L))
#define	iscntrl(c)	((__ctype + 1)[c] & _C)
#define	isdigit(c)	((__ctype + 1)[c] & _N)
#define	isgraph(c)	((__ctype + 1)[c] & (_P|_U|_L|_N))
#define	islower(c)	((__ctype + 1)[c] & _L)
#define	isprint(c)	((__ctype + 1)[c] & (_P|_U|_L|_N|_B))
#define	ispunct(c)	((__ctype + 1)[c] & _P)
#define	isspace(c)	((__ctype + 1)[c] & _S)
#define	isupper(c)	((__ctype + 1)[c] & _U)
#define	isxdigit(c)	((__ctype + 1)[c] & (_N|_X))
#define	tolower(c)	((__maplower + 1)[c])
#define	toupper(c)	((__mapupper + 1)[c])

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
#define	isascii(c)	((unsigned int)(c) <= 0177)
#define	isblank(c)	((c) == '\t' || (c) == ' ')
#define	toascii(c)	((c) & 0177)
#endif

#endif /* !_CTYPE_H_ */

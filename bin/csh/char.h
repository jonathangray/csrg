/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
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
 *	@(#)char.h	5.5 (Berkeley) 04/04/91
 */

extern unsigned short _cmap[];

#define	_Q	0x001		/* '" */
#define	_Q1	0x002		/* ` */
#define	_SP	0x004		/* space and tab */
#define	_NL	0x008		/* \n */
#define	_META	0x010		/* lex meta characters, sp #'`";&<>()|\t\n */
#define	_GLOB	0x020		/* glob characters, *?{[` */
#define	_ESC	0x040		/* \ */
#define	_DOL	0x080		/* $ */
#define	_DIG	0x100		/* 0-9 */
#define	_LET	0x200		/* a-z, A-Z, _ */

#define	cmap(c, bits)	(_cmap[(unsigned char)(c)] & (bits))

#define	isglob(c)	cmap(c, _GLOB)
#define	isspace(c)	cmap(c, _SP)
#define	isspnl(c)	cmap(c, _SP|_NL)
#define	ismeta(c)	cmap(c, _META)
#define	digit(c)	cmap(c, _DIG)
#define	letter(c)	cmap(c, _LET)
#define	alnum(c)	(digit(c) || letter(c))

#define	LINELEN		128
extern char *linp, linbuf[LINELEN];

#define	CSHPUTCHAR { \
	if (!(ch&QUOTE) && (ch == 0177 || ch < ' ' && ch != '\t' && \
	    ch != '\n')) { \
		*linp++ = '^'; \
		if (ch == 0177) \
			ch = '?'; \
		else \
			ch |= 'A' - 1; \
		if (linp >= &linbuf[sizeof linbuf - 2]) \
			flush(); \
	} \
	ch &= TRIM; \
	*linp++ = ch; \
	if (ch == '\n' || linp >= &linbuf[sizeof(linbuf) - 2]) \
		flush(); \
}

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
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
 *	@(#)cga.c	7.2 (Berkeley) 04/28/91
 */

typedef unsigned short u_short;
typedef unsigned char u_char;

#define	CRT_TXTADDR	((u_short *)0xb8000)
#define	COL		80
#define	ROW		25
#define	CHR		2

u_short	*crtat;
u_char	color = 0xe ;
int row;

sput(c) u_char c; {

	if (crtat == 0) {
		crtat = CRT_TXTADDR; bzero (crtat,COL*ROW*CHR);
	}
	if (crtat >= (CRT_TXTADDR+COL*ROW*CHR)) {
		crtat = CRT_TXTADDR+COL*(ROW-1); row = 0;
	}
	switch(c) {

	case '\t':
		do {
			*crtat++ = (color<<8)| ' '; row++ ;
		} while (row %8);
		break;

	case '\010':
		crtat--; row--;
		break;

	case '\r':
		bzero (crtat,(COL-row)*CHR) ; crtat -= row ; row = 0;
		break;

	case '\n':
		if (crtat >= CRT_TXTADDR+COL*(ROW-1)) { /* scroll */
			bcopy(CRT_TXTADDR+COL, CRT_TXTADDR,COL*(ROW-1)*CHR);
			bzero (CRT_TXTADDR+COL*(ROW-1),COL*CHR) ;
			crtat -= COL ;
		}
		crtat += COL ;
		break;

	default:
		*crtat++ = (color<<8)| c; row++ ;
		break ;
	}
}

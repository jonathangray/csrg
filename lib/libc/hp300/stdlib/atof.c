/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
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
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)atof.c	5.1 (Berkeley) 05/12/90";
#endif /* LIBC_SCCS and not lint */

#include <ctype.h>

double _twoemax =
#ifdef IEEE
	9007199254740992.;	/*2^53*/
#else
	72057594037927936.;	/*2^56*/
#endif

#ifdef hp300
/* attempt to be as exact as possible */
struct {
	long d_high;
	long d_low;
} _exp5[] = {
	{ 0x40140000, 0x00000000 },	/* 5 */
	{ 0x40390000, 0x00000000 },	/* 25 */
	{ 0x40838800, 0x00000000 },	/* 625 */
	{ 0x4117d784, 0x00000000 },	/* 390625 */
	{ 0x4241c379, 0x37e08000 },	/* 152587890625 */
	{ 0x4493b8b5, 0xb5056e17 },	/* 2.3283064365387e+022 */
	{ 0x49384f03, 0xe93ff9f6 },	/* 5.42101086242753e+044 */
	{ 0x52827748, 0xf9301d33 },	/* 2.93873587705572e+089 */
	{ 0x65154fdd, 0x7f73bf3f }	/* 8.63616855509445e+178 */
};
#else
double	_exp5[]	= {
	5.,
	25.,
	625.,
	390625.,
	152587890625.,
	23283064365386962890625.,
#ifdef IEEE
	5.4210108624275231e+044,
	2.9387358770557196e+089,
	8.6361685550944492e+178,
#endif
};
#endif

double
atof(p)
register char *p;
{
	extern double ldexp();
	register c, exp = 0, eexp = 0;
	double fl = 0, flexp = 1.0;
	int bexp, neg = 1, negexp = 1;

	while((c = *p++) == ' ');
	if (c == '-') neg = -1;	else if (c == '+'); else --p;

	while ((c = *p++), isdigit(c))
		if (fl < _twoemax) fl = 10*fl + (c-'0'); else exp++;
	if (c == '.')
	while ((c = *p++), isdigit(c))
		if (fl < _twoemax)
		{
			fl = 10*fl + (c-'0');
			exp--;
		}
	if ((c == 'E') || (c == 'e'))
	{
		if ((c= *p++) == '+'); else if (c=='-') negexp = -1; else --p;
		while ((c = *p++), isdigit(c)) eexp = 10*eexp + (c-'0');
		if (negexp < 0) eexp = -eexp; exp += eexp;
	}
	bexp = exp;
	if (exp < 0) exp = -exp;

	for (c = 0; c < sizeof(_exp5)/sizeof(_exp5[0]); c++)
	{
#ifdef hp300
		if (exp & 01) flexp *= *(double *)&_exp5[c];
#else
		if (exp & 01) flexp *= _exp5[c];
#endif
		exp >>= 1; if (exp == 0) break;
	}

	if (bexp < 0) fl /= flexp; else fl *= flexp;
	fl = ldexp(fl, bexp);
	if (neg < 0) return(-fl); else return(fl);
}

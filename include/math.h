/*
 * Copyright (c) 1985 The Regents of the University of California.
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
 * All recipients should regard themselves as participants in an ongoing
 * research project and hence should feel obligated to report their
 * experiences (good or bad) with these elementary function codes, using
 * the sendbug(8) program, to the authors.
 *
 *	@(#)math.h	5.3 (Berkeley) 02/24/91
 */

#if defined(vax) || defined(tahoe)
#define	HUGE_VAL	1.701411733192644270e38
#else
#define	HUGE_VAL	1e500	/* positive infinity */
#endif

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
#define	HUGE		HUGE_VAL
#endif

#include <sys/cdefs.h>

__BEGIN_DECLS
double	acos __P((double));
double	asin __P((double));
double	atan __P((double));
double	atan2 __P((double, double));
double	ceil __P((double));
double	cos __P((double));
double	cosh __P((double));
double	exp __P((double));
double	fabs __P((double));
double	floor __P((double));
double	fmod __P((double, double));
double	frexp __P((double, int *));
double	ldexp __P((double, int));
double	log __P((double));
double	log10 __P((double));
double	modf __P((double, double *));
double	pow __P((double, double));
double	sin __P((double));
double	sinh __P((double));
double	sqrt __P((double));
double	tan __P((double));
double	tanh __P((double));

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_SOURCE)
double	acosh __P((double));
double	asinh __P((double));
double	atanh __P((double));
double	cabs();		/* we can't describe cabs()'s argument properly */
double	cbrt __P((double));
double	copysign __P((double, double));
double	drem __P((double, double));
double	erf __P((double));
double	erfc __P((double));
double	expm1 __P((double));
int	finite __P((double));
double	hypot __P((double, double));
#if defined(vax) || defined(tahoe)
double	infnan __P((int));
#else
int	isnan __P((double));
#endif
double	j0 __P((double));
double	j1 __P((double));
double	jn __P((int, double));
double	lgamma __P((double));
double	log1p __P((double));
double	logb __P((double));
double	rint __P((double));
double	scalb __P((double, int));
double	y0 __P((double));
double	y1 __P((double));
double	yn __P((int, double));
#endif

__END_DECLS

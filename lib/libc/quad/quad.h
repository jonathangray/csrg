/*-
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
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
 *	@(#)quad.h	5.7 (Berkeley) 06/21/92
 */

/*
 * Quad arithmetic.
 *
 * This library makes the following assumptions:
 *
 *  - The type long long (aka quad) exists.
 *
 *  - A quad variable is exactly twice as long as `long'.
 *
 *  - The machine's arithmetic is two's complement.
 *
 * All other machine parameters are encapsulated here.  This library can
 * provide 128-bit arithmetic on a machine with 128-bit quads and 64-bit
 * longs, for instance, or 96-bit arithmetic on machines with 48-bit longs.
 */

#ifndef SPARC_XXX
#include <machine/endian.h>		/* see #else case */
#else
/*
 * These are for testing and for illustration: we expect <machine/endian.h>
 * to define these.  Actually, these match most big-endian machines; for
 * most little-endian machines, all you need do is exchange _QUAD_HIGHWORD
 * and _QUAD_LOWWORD.
 */
#define _QUAD_HIGHWORD	0
#define _QUAD_LOWWORD	1
#endif

typedef long long quad;
typedef unsigned long long u_quad;
typedef unsigned long u_long;

#include <limits.h>
/*
 * We expect something like these from <limits.h>, which should be provided on
 * any ANSI C system.
#define	USHRT_MAX	0xffff
#define	CHAR_BIT	8
 */

/*
 * Depending on the desired operation, we view a `long long' (aka quad) in
 * one or more of the following formats.
 */
union uu {
	quad	q;		/* as a (signed) quad */
	quad	uq;		/* as an unsigned quad */
	long	sl[2];		/* as two signed longs */
	u_long	ul[2];		/* as two unsigned longs */
};

/*
 * Define high and low longwords.
 */
#define	H		_QUAD_HIGHWORD
#define	L		_QUAD_LOWWORD

/*
 * Total number of bits in a quad and in the pieces that make it up.
 * These are used for shifting, and also below for halfword extraction
 * and assembly.
 */
#define	QUAD_BITS	(sizeof(quad) * CHAR_BIT)
#define	LONG_BITS	(sizeof(long) * CHAR_BIT)
#define	HALF_BITS	(sizeof(long) * CHAR_BIT / 2)

/*
 * Extract high and low shortwords from longword, and move low shortword of
 * longword to upper half of long, i.e., produce the upper longword of
 * ((quad)(x) << (number_of_bits_in_long/2)).  (`x' must actually be u_long.)
 *
 * These are used in the multiply code, to split a longword into upper
 * and lower halves, and to reassemble a product as a quad, shifted left
 * (sizeof(long)*CHAR_BIT/2).
 */
#define	HHALF(x)	((x) >> HALF_BITS)
#define	LHALF(x)	((x) & ((1 << HALF_BITS) - 1))
#define	LHUP(x)		((x) << HALF_BITS)

extern u_quad __qdivrem(u_quad u, u_quad v, u_quad *rem);

/*
 * XXX
 * Compensate for gcc 1 vs gcc 2.  Gcc 1 defines ?sh?di3's second argument
 * as u_quad, while gcc 2 correctly uses int.  Unfortunately, we still use
 * both compilers.
 */
#ifdef sparc
typedef unsigned int	qshift_t;
#else
typedef u_quad		qshift_t;
#endif

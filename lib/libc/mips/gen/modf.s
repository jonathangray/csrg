/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
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

#include "DEFS.h"

#if defined(LIBC_SCCS) && !defined(lint)
	ASMSTR("@(#)modf.s	5.2 (Berkeley) 02/29/92")
#endif /* LIBC_SCCS and not lint */

/*
 * double modf(val, iptr)
 *	double val, *iptr;
 * returns: xxx and n (in *iptr) where val == n.xxx
 */
LEAF(modf)
	li.d	$f6, 4503599627370496.0	# check for value out of range (2**52)
	abs.d	$f0, $f12
	c.lt.d	$f0, $f6
	mfc1	t0, $f13		# get the sign & exponent part
	bc1f	3f			# val is not less than maxint
	add.d	$f2, $f0, $f6		# logical shift right
	sub.d	$f2, $f2, $f6		# logical shift left
	c.le.d	$f2, $f0
	bc1t	1f
	li.d	$f6, 1.0		# adjust due to rounding
	sub.d	$f2, $f2, $f6
1:
	bge	t0, zero, 2f		# jump if val >= 0
	neg.d	$f2, $f0		# negate integer part if val < 0
2:
	s.d	$f2, 0(a2)		# save the integer part
	sub.d	$f0, $f12, $f2		# return the fractional part
	j	ra
3:
	mtc1	zero, $f0		# val was too big so
	mtc1	zero, $f1		#   return fraction of zero
	s.d	$f12, 0(a2)		#   and the original number.
	j	ra
END(modf)

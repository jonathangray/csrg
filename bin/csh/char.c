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
 */

#ifndef lint
static char sccsid[] = "@(#)char.c	5.4 (Berkeley) 04/04/91";
#endif /* not lint */

#include "sh.char.h"

unsigned short _cmap[256] = {
/*	nul		soh		stx		etx	*/
	0,		0,		0,		0,

/*	eot		enq		ack		bel	*/
	0,		0,		0,		0,

/*	bs		ht		nl		vt	*/
	0,		_SP|_META,	_NL|_META,	0,

/*	np		cr		so		si	*/
	0,		0,		0,		0,

/*	dle		dc1		dc2		dc3	*/
	0,		0,		0,		0,

/*	dc4		nak		syn		etb	*/
	0,		0,		0,		0,

/*	can		em		sub		esc	*/
	0,		0,		0,		0,

/*	fs		gs		rs		us	*/
	0,		0,		0,		0,

/*	sp		!		"		#	*/
	_SP|_META,	0,		_Q,		_META,

/*	$		%		&		'	*/
	_DOL,		0,		_META,		_Q,

/*	(		)		*		+	*/
	_META,		_META,		_GLOB,		0,

/*	,		-		.		/	*/
	0,		0,		0,		0,

/*	0		1		2		3	*/
	_DIG,		_DIG,		_DIG,		_DIG,

/*	4		5		6		7	*/
	_DIG,		_DIG,		_DIG,		_DIG,

/*	8		9		:		;	*/
	_DIG,		_DIG,		0,		_META,

/*	<		=		>		?	*/
	_META,		0,		_META,		_GLOB,

/*	@		A		B		C	*/
	0,		_LET,		_LET,		_LET,

/*	D		E		F		G	*/
	_LET,		_LET,		_LET,		_LET,

/*	H		I		J		K	*/
	_LET,		_LET,		_LET,		_LET,

/*	L		M		N		O	*/
	_LET,		_LET,		_LET,		_LET,

/*	P		Q		R		S	*/
	_LET,		_LET,		_LET,		_LET,

/*	T		U		V		W	*/
	_LET,		_LET,		_LET,		_LET,

/*	X		Y		Z		[	*/
	_LET,		_LET,		_LET,		_GLOB,

/*	\		]		^		_	*/
	_ESC,		0,		0,		_LET,

/*	`		a		b		c	*/
	_Q1|_GLOB,	_LET,		_LET,		_LET,

/*	d		e		f		g	*/
	_LET,		_LET,		_LET,		_LET,

/*	h		i		j		k	*/
	_LET,		_LET,		_LET,		_LET,

/*	l		m		n		o	*/
	_LET,		_LET,		_LET,		_LET,

/*	p		q		r		s	*/
	_LET,		_LET,		_LET,		_LET,

/*	t		u		v		w	*/
	_LET,		_LET,		_LET,		_LET,

/*	x		y		z		{	*/
	_LET,		_LET,		_LET,		_GLOB,

/*	|		}		~		del	*/
	_META,		0,		0,		0,
};

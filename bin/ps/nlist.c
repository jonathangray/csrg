/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
static char sccsid[] = "@(#)nlist.c	5.1 (Berkeley) 02/08/91";
#endif /* not lint */

#include <sys/types.h>
#include <nlist.h>
#include <errno.h>
#include <stdio.h>

struct	nlist psnl[] = {
	{"_ecmx"},
#define	X_ECMX		0
	{"_fscale"},
#define	X_FSCALE	1
	{"_ccpu"},
#define	X_CCPU		2
	{NULL}
};

fixpt_t	ccpu;				/* kernel _ccpu variable */
int	nlistread;			/* if nlist already read. */
int	ecmx;				/* kernel _ecmx variable */
int	fscale;				/* kernel _fscale variable */

#define kread(x, v) \
	kvm_read(psnl[x].n_value, (char *)&v, sizeof v) != sizeof(v)

donlist()
{
	extern int eval;
	int rval;

	rval = 0;
	nlistread = 1;
	if (kvm_nlist(psnl)) {
		(void)fprintf(stderr, "ps: kvm_nlist: %s\n", strerror(errno));
		eval = 1;
		return(1);
	}
	if (kread(X_FSCALE, fscale)) {
		(void)fprintf(stderr, "ps: fscale: %s\n", kvm_geterr());
		eval = rval = 1;
	}
	if (kread(X_ECMX, ecmx)) {
		(void)fprintf(stderr, "ps: ecmx: %s\n", kvm_geterr());
		eval = rval = 1;
	}
	if (kread(X_CCPU, ccpu)) {
		(void)fprintf(stderr, "ps: ccpu: %s\n", kvm_geterr());
		eval = rval = 1;
	}
	return(rval);
}


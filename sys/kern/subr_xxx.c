/*
 * Copyright (c) 1982, 1986, 1991 Regents of the University of California.
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
 *	@(#)subr_xxx.c	7.9 (Berkeley) 03/17/91
 */

/*
 * Miscellaneous trivial functions, including many
 * that are often inline-expanded or done in assembler.
 */
#include "types.h"
#include "machine/cpu.h"
#include "errno.h"

/*
 * Unsupported device function (e.g. writing to read-only device).
 */
enodev()
{

	return (ENODEV);
}

/*
 * Unconfigured device function; driver not configured.
 */
enxio()
{

	return (ENXIO);
}

/*
 * Return error for unsupported operation.
 */
eopnotsupp()
{

	return (EOPNOTSUPP);
}

/*
 * Generic null operation, always returns success.
 */
nullop()
{

	return (0);
}

/*
 * Definitions of various trivial functions;
 * usually expanded inline rather than being defined here.
 */
#if !defined(vax) && !defined(tahoe)
imin(a, b)
	int a, b;
{

	return (a < b ? a : b);
}

imax(a, b)
	int a, b;
{

	return (a > b ? a : b);
}

unsigned int
min(a, b)
	unsigned int a, b;
{

	return (a < b ? a : b);
}

unsigned int
max(a, b)
	unsigned int a, b;
{

	return (a > b ? a : b);
}

long
lmin(a, b)
	long a, b;
{

	return (a < b ? a : b);
}

long
lmax(a, b)
	long a, b;
{

	return (a > b ? a : b);
}

unsigned long
ulmin(a, b)
	unsigned long a, b;
{

	return (a < b ? a : b);
}

unsigned long
ulmax(a, b)
	unsigned long a, b;
{

	return (a > b ? a : b);
}
#endif

#if !defined(vax) && !defined(tahoe) && !defined(hp300)
ffs(mask)
	register long mask;
{
	register int bit;

	if (!mask)
		return(0);
	for (bit = 1;; ++bit) {
		if (mask&0x01)
			return(bit);
		mask >>= 1;
	}
}
#endif

#if !defined(vax) && !defined(hp300)
bcmp(v1, v2, len)
	void *v1, *v2;
	register unsigned len;
{
	register u_char *s1 = v1, *s2 = v2;

	while (len--)
		if (*s1++ != *s2++)
			return (1);
	return (0);
}

strlen(s1)
	register char *s1;
{
	register int len;

	for (len = 0; *s1++ != '\0'; len++)
		;
	return (len);
}
#endif

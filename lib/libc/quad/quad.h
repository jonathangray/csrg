/*-
 * Copyright (c) 1992 The Regents of the University of California.
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
 *	@(#)quad.h	5.3 (Berkeley) 05/12/92
 */

/* More subroutines needed by GCC output code on some machines.  */
/* Compile this one with gcc.  */
#include <sys/param.h>

#define	BITS_PER_WORD	(NBBY * sizeof(long))

/* We need this union to unpack/pack longlongs, since we don't have
   any arithmetic yet.  Incoming long long parameters are stored
   into the `ll' field, and the unpacked result is read from the struct
   longlong.  */

typedef union {
	long long ll;
	struct { long val[2]; } s;
} long_long;
#define high val[_QUAD_HIGHWORD]
#define low val[_QUAD_LOWWORD]

#define HIGH _QUAD_HIGHWORD
#define LOW _QUAD_LOWWORD

/* Internally, long long ints are strings of unsigned shorts in the
   order determined by BYTE_ORDER.  */

#define B 0x10000
#define low16 (B - 1)

#if BYTE_ORDER == BIG_ENDIAN
#define big_end(n)	0 
#define little_end(n)	((n) - 1)
#define next_msd(i)	((i) - 1)
#define next_lsd(i)	((i) + 1)
#define is_not_msd(i,n)	((i) >= 0)
#define is_not_lsd(i,n)	((i) < (n))
#else
#define big_end(n)	((n) - 1)
#define little_end(n)	0 
#define next_msd(i)	((i) + 1)
#define next_lsd(i)	((i) - 1)
#define is_not_msd(i,n)	((i) < (n))
#define is_not_lsd(i,n)	((i) >= 0)
#endif

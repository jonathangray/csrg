/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rick Macklem at The University of Guelph.
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
 *	@(#)xdr_subs.h	7.4 (Berkeley) 01/14/92
 */

/*
 * Macros used for conversion to/from xdr representation by nfs...
 * These use the MACHINE DEPENDENT routines ntohl, htonl
 * As defined by "XDR: External Data Representation Standard" RFC1014
 */
#if BYTE_ORDER == LITTLE_ENDIAN
#define fxdr_unsigned(t, v)	((t)ntohl((long)(v)))
#define	fxdr_time(f, t)	{ \
	((struct timeval *)(t))->tv_sec = \
		ntohl(((struct timeval *)(f))->tv_sec); \
	((struct timeval *)(t))->tv_usec = \
		ntohl(((struct timeval *)(f))->tv_usec); \
}

/*
 * To handle quad conversions, define a struct of two longs and use
 * ntohl and htonl. Maybe someday there should be ntohq and htonq?
 */
union _hq {
	quad_t	hq;
	struct {
		long val[2];
	} lq;
};
#define	fxdr_hyper(f, t) { \
	((union _hq *)(t))->lq.val[1] = ntohl(((union _hq *)(f))->lq.val[0]); \
	((union _hq *)(t))->lq.val[0] = ntohl(((union _hq *)(f))->lq.val[1]); \
}
#define	txdr_hyper(f, t) { \
	((union _hq *)(t))->lq.val[0] = htonl(((union _hq *)(f))->lq.val[1]); \
	((union _hq *)(t))->lq.val[1] = htonl(((union _hq *)(f))->lq.val[0]); \
}

#define	txdr_unsigned(v)	(htonl((long)(v)))
#define	txdr_time(f, t)	{ \
	((struct timeval *)(t))->tv_sec = \
		htonl(((struct timeval *)(f))->tv_sec); \
	((struct timeval *)(t))->tv_usec = \
		htonl(((struct timeval *)(f))->tv_usec); \
}
#else	/* BIG_ENDIAN */
#define fxdr_unsigned(t, v)	((t)(v))
#define	fxdr_time(f, t) \
	*((struct timeval *)(t)) = *((struct timeval *)(f))
#define	fxdr_hyper(f, t) \
	*((quad_t *)(t)) = *((quad_t *)(f))

#define	txdr_unsigned(v)	((long)(v))
#define	txdr_time(f, t) \
	*((struct timeval *)(t)) = *((struct timeval *)(f))
#define	txdr_hyper(f, t) \
	*((quad_t *)(t)) = *((quad_t *)(f))
#endif	/* ENDIAN */

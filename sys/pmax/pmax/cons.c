/*
 * Copyright (c) 1988 University of Utah.
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department and Ralph Campbell.
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
 * from: Utah $Hdr: cons.c 1.1 90/07/09$
 *
 *	@(#)cons.c	7.4 (Berkeley) 10/11/92
 */

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/file.h>
#include <sys/conf.h>

#include <machine/machMon.h>

/*
 * Console output may be redirected to another tty
 * (e.g. a window); if so, constty will point to the current
 * virtual console.
 */
struct	tty *constty;		/* virtual console output device */

/*
 * Get character from console.
 */
cngetc()
{
	int (*f)();
#include "dc.h"
#if NDC > 0
#include <machine/dc7085cons.h>
#include <pmax/dev/pdma.h>
	extern struct pdma dcpdma[];

	/* check to be sure device has been initialized */
	if (dcpdma[0].p_addr)
		return (dcKBDGetc());
	f = (int (*)())MACH_MON_GETCHAR;
	return (*f)();
#else
	f = (int (*)())MACH_MON_GETCHAR;
	return (*f)();
#endif
}

/*
 * Print a character on console.
 */
cnputc(c)
	register int c;
{
#include "pm.h"
#if NPM > 0
	pmPutc(c);
#else
#include "cfb.h"
#if NCFB > 0
	cfbPutc(c);
#else
	int s;
	void (*f)() = (void (*)())MACH_MON_PUTCHAR;

	s = splhigh();
	(*f)(c);
	splx(s);
#endif
#endif
}

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * Copying or redistribution in any form is explicitly forbidden
 * unless prior written permission is obtained from William Jolitz or an
 * authorized representative of the University of California, Berkeley.
 *
 * Freely redistributable copies of this code will be available in
 * the near future; for more information contact William Jolitz or
 * the Computer Systems Research Group at the University of California,
 * Berkeley.
 *
 * The name of the University may not be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 *	@(#)clock.c	5.1 (Berkeley) 04/24/90
 */

/*
 * Primitive clock interrupt routines.
 */
#include "param.h"
#include "time.h"
#include "kernel.h"
#include "icu.h"

startrtclock() {

	/* initialize 8253 clock */
	outb (0x43, 0x36);
	outb (0x40, 1193182/60);
	outb (0x40, (1193182/60)/256);
}

clkreld() {
pg("clkreld");
}

/*
 * Initialze the time of day register, based on the time base which is, e.g.
 * from a filesystem.
 */
inittodr(base)
	time_t base;
{

	time.tv_sec = base;
}

/*
 * Restart the clock.
 */
resettodr()
{
}

enablertclock() {
	INTREN(IRQ0);
	splnone();
}

/*-
 * Copyright (c) 1980, 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)fetch.c	5.6 (Berkeley) 07/23/92";
#endif /* not lint */

#include <sys/types.h>
#include "systat.h"
#include "extern.h"

int
kvm_ckread(a, b, l)
	void *a, *b;
	int l;
{
	if (kvm_read(kd, (u_long)a, b, l) != l) {
		if (verbose)
			error("error reading kmem at %x\n", a);
		return (0);
	} 
	else
		return (1);
}

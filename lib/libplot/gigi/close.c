/*-
 * Copyright (c) 1980 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)close.c	5.3 (Berkeley) 04/22/91";
#endif /* not lint */

#include <signal.h>
#include "gigi.h"

void
closepl()
{
	/* recieve interupts */
	signal(SIGINT, SIG_IGN);

	/* exit graphics mode */
	putchar( ESC );
	putchar('\\');

	exit(0);
}

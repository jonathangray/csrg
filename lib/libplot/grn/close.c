/*-
 * Copyright (c) 1980, 1986 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)close.c	6.2 (Berkeley) 04/22/91";
#endif /* not lint */

#include "grnplot.h"

/*---------------------------------------------------------
 *	Closepl ends the gremlin file
 *
 *	Results:	None.
 *
 *	Side Effects:
 *---------------------------------------------------------
 */
closepl()
{
    if (!ingrnfile) return;
    endvector();
    printf("-1\n");
    ingrnfile = 0;
}

/*-
 * Copyright (c) 1980 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)h_abs.c	5.2 (Berkeley) 04/12/91";
#endif /* not lint */

short h_abs(x)
short *x;
{
if(*x >= 0)
	return(*x);
return(- *x);
}

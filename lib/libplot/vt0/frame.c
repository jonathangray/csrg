/*-
 * Copyright (c) 1983 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)frame.c	4.2 (Berkeley) 04/22/91";
#endif /* not lint */

frame(n)
{
	extern vti;
	n=n&0377 | 02000;
	write(vti,&n,2);
}

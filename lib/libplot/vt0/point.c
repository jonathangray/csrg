/*-
 * Copyright (c) 1983 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)point.c	4.2 (Berkeley) 04/22/91";
#endif /* not lint */

extern vti;
extern xnow,ynow;
point(xi,yi){
	struct {char pad,c; int x,y;} p;
	p.c = 2;
	p.x = xnow = xsc(xi);
	p.y = ynow =  ysc(yi);
	write(vti,&p.c,5);
}

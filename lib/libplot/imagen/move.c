/*-
 * Copyright (c) 1985 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)move.c	5.2 (Berkeley) 04/22/91";
#endif /* not lint */

#include "imp.h"
move(xi,yi){
        imPx = xi;
        imPy = yi;
}

/*-
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)sibuf.c	5.3 (Berkeley) 04/20/91";
#endif /* LIBC_SCCS and not lint */

#include <stdio.h>

char	_sibuf[BUFSIZ];

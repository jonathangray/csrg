/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
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
 */

#ifndef lint
static char sccsid[] = "@(#)getbsize.c	5.2 (Berkeley) 03/09/92";
#endif /* not lint */

#include <stdio.h>
#include <stdlib.h>

char *
getbsize(prog, headerlenp, blocksizep)
	char *prog;
	int *headerlenp;
	long *blocksizep;
{
	static char header[20];
	long blocksize;
	char *ep, *p;

#define	Kb	1024
#define	Mb	1048576
#define	Gb	1073741824
	if ((p = getenv("BLOCKSIZE")) != NULL && *p != '\0') {
		blocksize = strtol(p, &ep, 10);
		switch(*ep) {
		case 'G': case 'g':
			if (ep[1])
				goto fmterr;
			if (blocksize > 1)
				goto overflow;
			*headerlenp = snprintf(header, sizeof(header),
			    "%dG-blocks", blocksize);
			*blocksizep = blocksize * Gb;
			return (header);
		case 'M': case 'm':
			if (ep[1])
				goto fmterr;
			*headerlenp = snprintf(header, sizeof(header),
			    "%dM-blocks", blocksize);
			*blocksizep = blocksize * Mb;
			return (header);
		case 'K': case 'k':
			if (ep[1])
				goto fmterr;
			*headerlenp = snprintf(header, sizeof(header),
			    "%dK-blocks", blocksize);
			*blocksizep = blocksize * Kb;
			return (header);
		case '\0':
			if (blocksize > Gb) {
overflow:			(void)fprintf(stderr,
				    "%s: maximum blocksize is 1G\n", prog);
				blocksize = 512;
			} else if (blocksize < 512) {
				(void)fprintf(stderr,
				    "%s: minimum blocksize is 512\n", prog);
				blocksize = 512;
			}
			break;
		default:
fmterr:			(void)fprintf(stderr,
			    "%s: %s: unknown blocksize\n", prog, p);
			blocksize = 512;
			break;
		}
	} else
		blocksize = 512;

	*headerlenp = snprintf(header, sizeof(header), "%d-blocks", blocksize);
	*blocksizep = blocksize;
	return (header);
}

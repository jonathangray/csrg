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
static char sccsid[] = "@(#)fortune.c	5.4 (Berkeley) 06/06/91";
#endif /* not lint */

/* fortune.c		 Larn is copyrighted 1986 by Noah Morgan. */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"
#include "pathnames.h"

/*
 * function to return a random fortune from the fortune file
 */

char *
fortune()
{
	static int fd = -1;	/* true if we have load the fortune info */
	static int nlines;	/* # lines in fortune database */
	register int tmp;
	register char *p;
	char *base, **flines;
	struct stat sb;

	if (fd != -1)
		return (flines[random() % nlines]);

	if ((fd = open(_PATH_FORTS, O_RDONLY)) < 0)
		return (NULL);

	/* Find out how big fortune file is and get memory for it. */
	if ((fstat(fd, &sb) < 0) ||
	    ((base = malloc(1 + sb.st_size)) == NULL)) {
		(void)close(fd);
		goto bad;
	}

	/* Read in the entire fortune file. */
	if (read(fd, base, sb.st_size) != sb.st_size) {
		free(base);
		goto bad;
	}
	base[sb.st_size] = '\0';	/* Final NULL termination. */

	(void)close(fd);


	/*
	 * Count up all the lines (and NULL terminate) to know memory
	 * needs.
	 */
	for (p = base; p < base + sb.st_size; p++)
		if (*p == '\n') {
			*p = '\0';
			++nlines;
		}

	if (nlines <= 0) {
		free(base);
		goto bad;
	}

	/* Get memory for array of pointers to each fortune. */
	if ((flines = malloc(nlines * sizeof(char *))) == NULL) {
		free(base);
		goto bad;
	}

	/* Now assign each pointer to a line. */
	for (p = base, tmp = 0; tmp < nlines; ++tmp) {
		flines[tmp] = p;
		while (*p++);
	}

	return (flines[random() % nlines]);

bad:	fd = -1;
	return (NULL);
}

/*
 * Copyright (c) 1992, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley
 * by Pace Willisson (pace@blitz.com).  The Rock Ridge Extension
 * Support code is derived from software contributed to Berkeley
 * by Atsushi Murai (amurai@spec.co.jp).
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
 *
 *      @(#)mount_cd9660.c	8.3 (Berkeley) 02/18/94
 */

#ifndef lint
static char copyright[] =
"@(#) Copyright (c) 1992, 1993\n\
        The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)mount_cd9660.c	8.3 (Berkeley) 02/18/94";
#endif /* not lint */

#include <sys/param.h>
#define CD9660
#include <sys/mount.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_ROOTUID	-2

void
usage()
{

	fprintf(stderr,
		"usage: mount_cd9660 [-F fsoptions] [-norrip] [-gen] [-extattr] special node\n");
	exit(1);
}
		
int
main(argc, argv)
	int argc;
	char **argv;
{
	char *dev;
	char *dir;
	struct iso_args args;
	int mntflags = 0, opts = 0;

	argc--;
	argv++;
	while (argc > 2) {
		if (!strcmp("-F", argv[0])) {
			argc--; argv++;
			mntflags |= atoi(argv[0]);
			argc--; argv++;
		} else if (!strcmp(argv[0], "-norrip")) {
			opts |= ISOFSMNT_NORRIP;
			argc--; argv++;
		} else if (!strcmp(argv[0], "-gen")) {
			opts |= ISOFSMNT_GENS;
			argc--; argv++;
		} else if (!strcmp(argv[0], "-extattr")) {
			opts |= ISOFSMNT_EXTATT;
			argc--; argv++;
		} else if (!strcmp(argv[0], "-notrans")) {
			opts |= ISOFSMNT_NOTRANS;
			argc--; argv++;
		} else
			usage();
	}

	dev = argv[0];
	dir = argv[1];

	args.fspec = dev;
	args.export.ex_root = DEFAULT_ROOTUID;
	if (mntflags & MNT_RDONLY)
		args.export.ex_flags = MNT_EXRDONLY;
	else
		args.export.ex_flags = 0;
	args.flags = opts;

	if (mount(MOUNT_CD9660, dir, mntflags, &args) < 0) {
		(void)fprintf(stderr, "mount_cd9660: %s\n", strerror(errno));
		exit(1);
	}

	exit(0);
}

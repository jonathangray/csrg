/*
 * Copyright (c) 1992, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software donated to Berkeley by
 * Jan-Simon Pendry.
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
 *	@(#)mount_union.c	8.2 (Berkeley) 02/17/94
 */

#include <sys/param.h>
#include <sys/mount.h>
#include <miscfs/union/union.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static int subdir __P((char *, char *));
void usage __P((void));

int
main(argc, argv)
	int argc;
	char *argv[];
{
	struct union_args args;
	int ch, mntflags;
	char target[MAXPATHLEN];
	int error = 0;

	mntflags = 0;
	args.mntflags = UNMNT_ABOVE;

	while ((ch = getopt(argc, argv, "F:a:b:r:")) != EOF) {
		switch(ch) {
		case 'F':
			mntflags = atoi(optarg);
			break;
		case 'a':
			if (strcmp(optarg, "bove") == 0) {
				args.mntflags &= ~UNMNT_OPMASK;
				args.mntflags |= UNMNT_ABOVE;
			} else {
				error = 1;
			}
			break;

		case 'b':
			if (strcmp(optarg, "elow") == 0) {
				args.mntflags &= ~UNMNT_OPMASK;
				args.mntflags |= UNMNT_BELOW;
			} else {
				error = 1;
			}
			break;

		case 'r':
			if (strcmp(optarg, "eplace") == 0) {
				args.mntflags &= ~UNMNT_OPMASK;
				args.mntflags |= UNMNT_REPLACE;
			} else {
				error = 1;
			}
			break;

		case '?':
		default:
			error = 1;
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 2)
		error = 1;

	if (error)
		usage();

	if (realpath(argv[0], target) == 0) {
		(void)fprintf(stderr, "mount_union: %s: %s\n",
				target, strerror(errno));
		exit(1);
	}

	if (subdir(target, argv[1])) {
		(void)fprintf(stderr,
			"mount_union: %s is a sub-directory of %s\n",
				argv[0], argv[1]);
		exit(1);
	}

	args.target = target;

	if (mount(MOUNT_UNION, argv[1], mntflags, &args)) {
		(void)fprintf(stderr, "mount_union: %s\n", strerror(errno));
		exit(1);
	}
	exit(0);
}

static int
subdir(p, dir)
	char *p;
	char *dir;
{
	int l;

	l = strlen(dir);
	if (l <= 1)
		return (1);

	if ((strncmp(p, dir, l) == 0) && (p[l] == '/'))
		return (1);

	return (0);
}

void
usage()
{
	(void)fprintf(stderr,
	    "usage: mount_union [ -F fsoptions ] target_fs mount_point\n");
	exit(1);
}

/*-
 * Copyright (c) 1988 The Regents of the University of California.
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
char copyright[] =
"@(#) Copyright (c) 1988 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)chpass.c	5.16 (Berkeley) 02/12/91";
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <pwd.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "chpass.h"
#include "pathnames.h"

char *progname = "chpass";
char *tempname;
uid_t uid;

main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind;
	extern char *optarg;
	register enum { NEWSH, LOADENTRY, EDITENTRY } op;
	register struct passwd *pw;
	struct passwd lpw;
	int ch, pfd, tfd;
	char *arg;

	op = EDITENTRY;
	while ((ch = getopt(argc, argv, "a:s:")) != EOF)
		switch(ch) {
		case 'a':
			op = LOADENTRY;
			arg = optarg;
			break;
		case 's':
			op = NEWSH;
			arg = optarg;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	uid = getuid();

	if (op == EDITENTRY || op == NEWSH)
		switch(argc) {
		case 0:
			if (!(pw = getpwuid(uid))) {
				(void)fprintf(stderr,
				    "chpass: unknown user: uid %u\n", uid);
				exit(1);
			}
			break;
		case 1:
			if (!(pw = getpwnam(*argv))) {
				(void)fprintf(stderr,
				    "chpass: unknown user %s.\n", *argv);
				exit(1);
			}
			if (uid && uid != pw->pw_uid)
				baduser();
			break;
		default:
			usage();
		}

	if (op == NEWSH) {
		/* protect p_shell -- it thinks NULL is /bin/sh */
		if (!arg[0])
			usage();
		if (p_shell(arg, pw, (ENTRY *)NULL))
			pw_error((char *)NULL, 0, 1);
	}

	if (op == LOADENTRY) {
		if (uid)
			baduser();
		pw = &lpw;
		if (!pw_scan(arg, pw))
			exit(1);
	}

	/*
	 * The file descriptor usage is a little tricky through here.
	 * 1:	We start off with two fd's, one for the master password
	 *	file, and one for the temporary file.
	 * 2:	Get an fp for the temporary file, copy the info to be
	 *	edited into it, and close the fp (closing the underlying
	 *	fd).
	 * 3:	The user edits the temporary file some number of times.
	 * 4:	Get an fp for the temporary file, and verify the contents.
	 *	We can't use the fp from step 2, because the user's editor
	 *	may have created a new instance of the file.  Close the
	 *	fp when done.
	 * 5:	Get an fp for the temporary file, truncating it as we do
	 *	so.  Get an fp for the master password file.  Copy the
	 *	master password file into the temporary file, replacing the
	 *	user record with a new one.  Close the temporary file fp
	 *	when done -- can't close the password fp, or we'd lose the
	 *	lock.
	 * 6:	Call pw_mkdb() and exit.  The exit closes the master password
	 *	fd from step 1, and the master password fp from step 5.
	 */
	pw_init();
	pfd = pw_lock();
	tfd = pw_tmp();

	if (op == EDITENTRY) 
		edit(tfd, pw);

	pw_copy(pfd, pw);

	if (!pw_mkdb())
		pw_error((char *)NULL, 0, 1);
	exit(0);
}

baduser()
{
	(void)fprintf(stderr, "chpass: %s\n", strerror(EACCES));
	exit(1);
}

usage()
{
	(void)fprintf(stderr, "usage: chpass [-a list] [-s shell] [user]\n");
	exit(1);
}

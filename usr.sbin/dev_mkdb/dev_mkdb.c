/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
"@(#) Copyright (c) 1990 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)dev_mkdb.c	5.1 (Berkeley) 02/12/91";
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <ndbm.h>
#include <kvm.h>
#include <errno.h>
#include <stdio.h>
#include <paths.h>
#include <string.h>

main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind;
	register DIR *dirp;
	register struct dirent *dp;
	struct stat sb;
	DBM *db;
	datum key, data;
	int ch;
	char buf[MAXNAMLEN + 1], dbtmp[MAXPATHLEN + 1], dbname[MAXPATHLEN + 1];

	while ((ch = getopt(argc, argv, "")) != EOF)
		switch((char)ch) {
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	if (chdir(_PATH_DEV))
		error(_PATH_DEV);

	dirp = opendir(".");

	(void)sprintf(dbtmp, "%s/dev.db.tmp", _PATH_VARRUN);
	(void)sprintf(dbname, "%s/dev.db", _PATH_VARRUN);
	if ((db = dbm_open(dbtmp, O_CREAT|O_WRONLY|O_EXCL,
	    S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == NULL)
		error(dbtmp);

	key.dptr = (char *)&sb.st_rdev;
	key.dsize = sizeof(sb.st_rdev);
	data.dptr = buf;
	while (dp = readdir(dirp)) {
		if (stat(dp->d_name, &sb))
			error(dp->d_name);
		if (!S_ISCHR(sb.st_mode))
			continue;

		/* Nul terminate the name so ps doesn't have to. */
		bcopy(dp->d_name, buf, dp->d_namlen);
		buf[dp->d_namlen] = '\0';
		data.dsize = dp->d_namlen + 1;
		if (dbm_store(db, key, data, DBM_INSERT) < 0)
			error("dbm_store");
	}
	(void)dbm_close(db);
	if (rename(dbtmp, dbname)) {
		(void)fprintf(stderr, "dev_mkdb: %s to %s: %s.\n",
		    dbtmp, dbname, strerror(errno));
		exit(1);
	}
	exit(0);
}

error(n)
	char *n;
{
	(void)fprintf(stderr, "kvm_mkdb: %s: %s\n", n, strerror(errno));
	exit(1);
}

usage()
{
	(void)fprintf(stderr, "usage: dev_mkdb\n");
	exit(1);
}

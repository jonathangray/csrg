/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Newcomb.
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
"@(#) Copyright (c) 1989 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)du.c	5.6 (Berkeley) 06/01/90";
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

typedef struct _ID {
	dev_t	dev;
	ino_t	inode;
} ID;

ID *files;
dev_t device;
int crossmounts, kvalue, listdirs, listfiles, maxfiles, numfiles;
char path[MAXPATHLEN + 1];

main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind, errno;
	int ch;
	char *malloc(), top[MAXPATHLEN + 1];

	listdirs = crossmounts = 1;
	while ((ch = getopt(argc, argv, "aksx")) != EOF)
		switch(ch) {
		case 'a':
			listfiles = 1;
			break;
		case 'k':
			kvalue = 1;
			break;
		case 's':
			listfiles = listdirs = 0;
			break;
		case 'x':
			crossmounts = 0;
			break;
		case '?':
		default:
			(void)fprintf(stderr,
			    "usage: du [-aksx] [name ...]\n");
			exit(1);
		}
	argv += optind;

	files = (ID *)malloc((u_int)(sizeof(ID) * (maxfiles = 128)));

	if (!*argv)
		du(".");
	else {
		if (argv[1])
			(void)getwd(top);
		for (;;) {
			du(*argv);
			if (!*++argv)
				break;
			if (chdir(top)) {
				(void)fprintf(stderr, "du: %s: %s\n",
				    top, strerror(errno));
				exit(1);
			}
		}
	}
	exit(0);
}

struct stat info;

du(arg)
	register char *arg;
{
	extern int errno;
	u_long total, descend();

	if (lstat(arg, &info)) {
		(void)fprintf(stderr, "du: %s: %s\n", arg, strerror(errno));
		return;
	}
	if ((info.st_mode&S_IFMT) != S_IFDIR) {
		(void)printf("%ld\t%s\n", kvalue ?
		    howmany(info.st_blocks, 2) : info.st_blocks, arg);
		return;
	}
	device = info.st_dev;
	(void)strcpy(path, arg);
	total = descend(path);
	if (!listfiles && !listdirs)
		(void)printf("%lu\t%s\n",
		    kvalue ? howmany(total, 2) : total, path);
}

u_long
descend(endp)
	register char *endp;
{
	extern int errno;
	register DIR *dir;
	register ID *fp;
	register struct dirent *dp;
	u_long total;
	char *realloc();

	if (info.st_nlink > 1) {
		for (fp = files + numfiles - 1; fp >= files; --fp)
			if (info.st_ino == fp->inode &&
			    info.st_dev == fp->dev)
				return(0L);
		if (numfiles == maxfiles)
			files = (ID *)realloc((char *)files,
			    (u_int)(sizeof(ID) * (maxfiles += 128)));
		files[numfiles].inode = info.st_ino;
		files[numfiles].dev = info.st_dev;
		++numfiles;
	}
	total = info.st_blocks;
	if ((info.st_mode&S_IFMT) == S_IFDIR) {
		if (info.st_dev != device && !crossmounts)
			return(0L);
		if (chdir(endp) || !(dir = opendir("."))) {
			(void)fprintf(stderr, "du: %s: %s\n",
			    path, strerror(errno));
			return(total);
		} 
		for (; *endp; ++endp);
		if (endp[-1] != '/')
			*endp++ = '/';
		while (dp = readdir(dir)) {
			if (dp->d_name[0] == '.' && (!dp->d_name[1] ||
			    dp->d_name[1] == '.' && !dp->d_name[2]))
				continue;
			bcopy(dp->d_name, endp, dp->d_namlen + 1);
			if (lstat(dp->d_name, &info)) {
				(void)fprintf(stderr, "du: %s: %s\n", path,
				    strerror(errno));
				continue;
			}
			total += descend(endp);
		}
		closedir(dir);
		if (chdir("..")) {
			(void)fprintf(stderr, "du: ..: %s\n", strerror(errno));
			exit(1);
		}
		*--endp = '\0';
		if (listdirs)
			(void)printf("%lu\t%s\n",
			    kvalue ? howmany(total, 2) : total, path);
	}
	else if (listfiles)
		(void)printf("%lu\t%s\n",
		    kvalue ? howmany(total, 2) : total, path);
	return(total);
}

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Michael Fischbein.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1989 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)ls.c	5.14 (Berkeley) 06/26/89";
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <dirent.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include "ls.h"

int lstat(), strlen(), prablelen();
char *emalloc();

int	qflg, Aflg, Cflg, Fflg, Lflg, Rflg, Sflg;

/* flags */
int f_listdot;			/* list files beginning with . */
int f_listalldot;		/* list . and .. as well */
int f_modtime;			/* use time of last change for time */
int f_accesstime;		/* use time of last access */
int f_statustime;		/* use time of last mode change */
int f_singlecol;		/* use single column output */
int f_listdir;			/* list actual directory, not contents */
int f_specialdir;		/* force params to be directories */
int f_type;			/* add type character for non-regular files */
int f_group;			/* show group ownership of a file */
int f_inode;			/* print inode */
int f_longform;			/* long listing format */
int f_nonprint;			/* show unprintables as ? */
int f_reversesort;		/* reverse whatever sort is used */
int f_recursive;		/* ls subdirectories also */
int f_size;			/* list size in short listing */
int f_timesort;			/* sort by time vice name */
int f_firsttime = 1;		/* to control recursion */

main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind, stat();
	int ch;
	int namecmp(), revnamecmp(), acccmp(), revacccmp();
	int modcmp(), revmodcmp(), statcmp(), revstatcmp();

	 /* set up defaults for terminal/nonterminal stdout */
	if (isatty(1)) {
		f_nonprint = 1;
		lengthfcn = prablelen;
	} else
		f_singlecol = 1;

	/* root sees all files automatically */
	if (!getuid())
		f_listdot = 1;

	while ((ch = getopt(argc, argv, "1ACFLRacdfgilqrstu")) != EOF) {
		switch (ch) {
		case '1':
			f_singlecol = 1;
			break;
		case 'C':
			f_singlecol = 0;
			break;
		case 'F':
			f_type = 1;
			break;
		case 'L':
			statfcn = stat;
			break;
		case 'R':
			f_recursive = 1;
			statfcn = lstat;
			break;
		case 'a':
			f_listalldot = 1;
			/* FALLTHROUGH */
		case 'A':
			f_listdot = 1;
			break;
		case 'c':
			f_statustime = 1;
			f_modtime = f_accesstime = 0;
			break;
		case 'S':
			Sflg++; /* fall into... */
		case 'd':
			f_listdir = 1;
			break;
		case 'f':
			f_specialdir = 1;
			break;
		case 'g':
			f_group = 1;
			break;
		case 'i':
			f_inode = 1;
			break;
		case 'l':
			f_longform = 1;
			f_singlecol = 0;
			statfcn = lstat;
			if (!f_accesstime && !f_statustime)
				f_modtime = 1;
			break;
		case 'q':
			f_nonprint = 1;
			lengthfcn = prablelen;
			break;
		case 'r':
			f_reversesort = 1;
			break;
		case 's':
			f_size = 1;
			statfcn = lstat;
			break;
		case 't':
			f_timesort = 1;
			if (!f_accesstime && !f_statustime)
				f_modtime = 1;
			break;
		case 'u':
			f_modtime = f_statustime = 0;
			f_accesstime = 1;
			break;
		default:
		case '?':
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	/* -f turns off -l, -t, -s, -r, turns on -a */
	if (f_specialdir) {
		f_longform = f_timesort = f_size = f_reversesort =
		f_recursive = f_type = 0;
		f_listdot = f_listalldot = 1;
	}

	/* -d turns off -R */
	if (f_listdir)
		f_recursive = 0;

	/* if will need to stat files */
	needstat = f_longform || f_recursive || f_timesort || f_size ||
	    f_inode || f_type;

	/* select a sort function */
	if (f_reversesort) {
		if (!f_timesort)
			sortfcn = revnamecmp;
		else if (f_accesstime)
			sortfcn = revacccmp;
		else if (f_modtime)
			sortfcn = revmodcmp;
		else if (f_statustime)
			sortfcn = revstatcmp;
	} else {
		if (!f_timesort)
			sortfcn = namecmp;
		else if (f_accesstime)
			sortfcn = acccmp;
		else if (f_modtime)
			sortfcn = modcmp;
		else if (f_statustime)
			sortfcn = statcmp;
	}

	if (argc)
		args(argc, argv);
	else
		curdir();
	exit(0);
}

/**
 * curdir --
 *	no arguments, list the current directory; this is the
 *	most common case, so it must run labooh.
 */
curdir()
{
	LS local, *stats;
	int num;
	char *names;

	if (statfcn(local.name = ".", &local.lstat)) {
		(void)fprintf(stderr, "ls: .: %s\n", strerror(errno));
		exit(1);
	}
	if (num = buildstats(&local, &stats, &names))
		ls(stats, num);
}

/**
 * args --
 *	handle arguments; this is separated out since we want to do the
 *	files first, then the directories, plus we must return to the
 *	current directory each time.
 */
args(argc, argv)
	int argc;
	char **argv;
{
	register LS *dstats, *rstats;
	register int cnt, dircnt, regcnt;
	struct stat sb;
	LS *stats;
	int num;
	char *names;

	dstats = rstats = NULL;
	for (dircnt = regcnt = 0; *argv; ++argv) {
		if (statfcn(*argv, &sb)) {
			(void)fprintf(stderr, "ls: %s: %s\n",
			    *argv, strerror(errno));
			if (errno == ENOENT)
				continue;
			exit(1);
		}
		if (!f_specialdir && !f_listdir && S_ISDIR(sb.st_mode)) {
			if (!dstats)
				dstats = (LS *)emalloc((u_int)argc *
				    (sizeof(LS)));
			dstats[dircnt].name = *argv;
			dstats[dircnt].lstat = sb;
			++dircnt;
		}
		else {
			if (!rstats)
				rstats = (LS *)emalloc((u_int)argc *
				    (sizeof(LS)));
			rstats[regcnt].name = *argv;
			rstats[regcnt].lstat = sb;
			++regcnt;
		}
	}
	if (regcnt) {
		if (f_specialdir) {
			for (cnt = 0; cnt < regcnt; ++cnt) {
				if (num = buildstats(rstats++, &stats, &names))
					ls(stats, num);
				(void)free((char *)stats);
				(void)free((char *)names);
			}
		} else
			ls(rstats, regcnt);
		if (dircnt)
			(void)putchar('\n');
	}
	if (dircnt) {
		if (dircnt > 1)
			qsort((char *)dstats, dircnt, sizeof(LS), sortfcn);
		for (cnt = 0; cnt < dircnt; ++cnt)
			ls_dir(dstats++, cnt, regcnt || dircnt > 1);
	}
#ifdef whybother
	(void)free((char *)rstats);
	(void)free((char *)dstats);
#endif
}

ls(stats, num)
	LS *stats;
	register int num;
{
	register char *p;
	LS *lp;

	if (num > 1 && !f_specialdir)
		qsort((char *)stats, num, sizeof(LS), sortfcn);

	printdir(stats, num);

	if (f_recursive)
		for (lp = stats; num--; ++lp) {
			if (!S_ISDIR(lp->lstat.st_mode))
				continue;
			p = lp->name;
			if (p[0] == '.' && (!p[1] || p[1] == '.' && !p[2]))
				continue;
			ls_dir(lp, 1, 1);
		}
}

ls_dir(lp, newline, tag)
	LS *lp;
	int newline, tag;
{
	static char path[MAXPATHLEN + 1];
	register char *p;
	LS *stats;
	int num;
	char *endofname, *names;

	/* add current name to path */
	p = index(path, '\0');
	if (p != path && p[-1] != '/')
		*p++ = '/';
	(void)strcpy(p, lp->name);
	endofname = p;

	if (newline)
		(void)putchar('\n');
	if (tag)
		(void)printf("%s:\n", path);

	if (chdir(lp->name)) {
		(void)fprintf(stderr, "ls: %s: %s\n",
		    lp->name, strerror(errno));
		return;
	}
	if (num = buildstats(lp, &stats, &names))
		ls(stats, num);

	*endofname = '\0';
	(void)free((char *)stats);
	(void)free((char *)names);
	if (chdir("..")) {
		(void)fprintf(stderr, "ls: ..: %s\n", strerror(errno));
		exit(1);
	}
}

static
buildstats(lp, s_stats, s_names)
	LS *lp, **s_stats;
	char **s_names;
{
	register int cnt, maxentry;
	register char *p, *names;
	struct dirent *entry;
	LS *stats;
	DIR *dirp;

	/* make this big so we don't realloc often */
#define	DEFNUM	256
	maxentry = DEFNUM;
	*s_stats = stats = (LS *)emalloc((u_int)DEFNUM * sizeof(LS));
	*s_names = names = emalloc((u_int)lp->lstat.st_size);

	if (!(dirp = opendir(f_specialdir ? lp->name : "."))) {
		(void)fprintf(stderr, "ls: %s: %s\n",
		    lp->name, strerror(errno));
		return(0);
	}
	for (cnt = 0; entry = readdir(dirp);) {
		p = entry->d_name;
		if (p[0] == '.') {
			if (!f_listdot)
				continue;
			if (!f_listalldot && (!p[1] || p[1] == '.' && !p[2]))
				continue;
		}
		if (cnt == maxentry) {
			maxentry += DEFNUM;
			if (!(stats = (LS *)realloc((char *)stats,
			    (u_int)maxentry * sizeof(LS))))
				nomem();
		}
		if (needstat && statfcn(entry->d_name, &stats[cnt].lstat)) {
			(void)fprintf(stderr, "ls: %s: %s\n",
			    entry->d_name, strerror(errno));
			if (errno == ENOENT)
				continue;
			exit(1);
		}
		stats[cnt].name = names;
		bcopy(entry->d_name, names, (int)entry->d_namlen);
		names += entry->d_namlen;
		*names++ = '\0';
		++cnt;
	}
	return(cnt);
}

/*
 * Copyright (c) 1980, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
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
static char copyright[] =
"@(#) Copyright (c) 1980, 1989, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)mount.c	8.9 (Berkeley) 02/17/94";
#endif /* not lint */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fstab.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include "pathnames.h"

#define DEFAULT_ROOTUID	-2

#define	BADTYPE(type) \
	(strcmp(type, FSTAB_RO) && strcmp(type, FSTAB_RW) && \
	    strcmp(type, FSTAB_RQ))
#define	SETTYPE(type) \
	(!strcmp(type, FSTAB_RW) || !strcmp(type, FSTAB_RQ))

int debug, force, verbose, updateflg, mnttype;
char *mntname;
char **vfslist;
static int badvfstype __P((int, char **));
static int badvfsname __P((char *, char **));
static int getexecopts __P((char *, char **));
static struct statfs *getmntpt __P((char *));
static int getmnttype __P((char *));
static void getstdopts __P((char *, int *));
static void getufsopts __P((char *, int *));
static char **makevfslist __P((char *));
static int mountfs __P((char *, char *, int, char *, char *, char *));
static void prmount __P((char *, char *, int));
static void usage __P((void));

int
main(argc, argv)
	int argc;
	char **argv;
{
	struct fstab *fs;
	int all, ch, rval, flags, ret, pid, i;
	long mntsize;
	struct statfs *mntbuf;
	char *type, *options = NULL;
	FILE *pidfile;

	all = 0;
	type = NULL;
	mnttype = MOUNT_UFS;
	mntname = "ufs";
	while ((ch = getopt(argc, argv, "adfrwuvt:o:")) != EOF)
		switch((char)ch) {
		case 'a':
			all = 1;
			break;
		case 'd':
			debug = 1;
			break;
		case 'f':
			force = 1;
			break;
		case 'r':
			type = FSTAB_RO;
			break;
		case 'u':
			updateflg = MNT_UPDATE;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'w':
			type = FSTAB_RW;
			break;
		case 'o':
			options = optarg;
			break;
		case 't':
			vfslist = makevfslist(optarg);
			mnttype = getmnttype(optarg);
			break;
		case '?':
		default:
			usage();
			/* NOTREACHED */
		}
	argc -= optind;
	argv += optind;

	/* NOSTRICT */

	if (all) {
		rval = 0;
		while (fs = getfsent()) {
			if (BADTYPE(fs->fs_type))
				continue;
			if (badvfsname(fs->fs_vfstype, vfslist))
				continue;
			/* `/' is special, it's always mounted */
			mnttype = getmnttype(fs->fs_vfstype);
			rval |= mountfs(fs->fs_spec, fs->fs_file, updateflg,
			    type, options, fs->fs_mntops);
		}
		exit(rval);
	}

	if (argc == 0) {
		if (verbose || debug || type)
			usage();
		if ((mntsize = getmntinfo(&mntbuf, MNT_NOWAIT)) == 0) {
			errx(1, "cannot get mount information");
		}
		for (i = 0; i < mntsize; i++) {
			if (badvfstype(mntbuf[i].f_type, vfslist))
				continue;
			prmount(mntbuf[i].f_mntfromname, mntbuf[i].f_mntonname,
				mntbuf[i].f_flags);
		}
		exit(0);
	}

	if (argc == 1 && vfslist) {
		usage();
	}

	if (argc == 1 && updateflg) {
		if ((mntbuf = getmntpt(*argv)) == NULL) {
			errx(1, "unknown special file or file system %s.",
			    *argv);
		}
		mnttype = mntbuf->f_type;
		if ((fs = getfsfile(mntbuf->f_mntonname)) == NULL) {
			errx(1, "can't find fstab entry for %s.", *argv);
		}
		mntname = fs->fs_vfstype;

		/*
		 * Default type to fstab version if none specified on the
		 * command line.
		 */
		if (type == NULL)
			type = fs->fs_type;

		/*
		 * Default options to fstab version if none specified on the
		 * command line.
		 */
		if (options == NULL)
			options = fs->fs_mntops;
		else {
			char *cp;

			/*
			 * Concat the two strings with the command line
			 * options last so that they will override the
			 * fstab options.
			 */
			i = strlen(fs->fs_mntops) + strlen(options) + 2;
			if ((cp = malloc((size_t)i)) == NULL) {
				errx(1, "-u malloc failed");
			}
			sprintf(cp, "%s,%s", fs->fs_mntops, options);
			options = cp;
		}
		ret = mountfs(fs->fs_spec, mntbuf->f_mntonname,
		    updateflg, type, options, (char *)NULL);
	} else if (argc == 1) {
		if (!(fs = getfsfile(*argv)) && !(fs = getfsspec(*argv))) {
			errx(1, "unknown special file or file system %s.\n",
			    *argv);
		}
		if (BADTYPE(fs->fs_type)) {
			errx(1, "%s has unknown file system type.\n", *argv);
		}
		mnttype = getmnttype(fs->fs_vfstype);
		ret = mountfs(fs->fs_spec, fs->fs_file, updateflg,
		    type, options, fs->fs_mntops);
	} else if (argc != 2) {
		usage();
		ret = 1;
	} else {
		/*
		 * If -t flag has not been specified, and spec
		 * contains either a ':' or a '@' then assume that
		 * an NFS filesystem is being specified ala Sun.
		 */
		if (vfslist == (char **)0 &&
		    (strchr(argv[0], ':') || strchr(argv[0], '@'))) {
			mnttype = MOUNT_NFS;
			mntname = "nfs";
		}
		ret = mountfs(argv[0], argv[1], updateflg, type, options,
		    (char *)NULL);
	}
	if ((pidfile = fopen(_PATH_MOUNTDPID, "r")) != NULL) {
		pid = 0;
		fscanf(pidfile, "%d", &pid);
		fclose(pidfile);
		if (pid > 0)
			kill(pid, SIGHUP);
	}
	exit(ret);
}

static int
mountfs(spec, name, flags, type, options, mntopts)
	char *spec, *name, *type, *options, *mntopts;
	int flags;
{
	int status;
	pid_t pid;
	int argc, i;
	struct ufs_args args;
	char *argp, *argv[50];
	char execname[MAXPATHLEN + 1], flagval[12];
	char mntpath[MAXPATHLEN];

	if (mntopts)
		getstdopts(mntopts, &flags);
	if (options)
		getstdopts(options, &flags);
	if (type)
		getstdopts(type, &flags);
	if (force)
		flags |= MNT_FORCE;

	if (realpath(name, mntpath) == 0) {
		warn("%s", mntpath);
		return (1);
	}

	name = mntpath;

	if (strcmp(name, "/") == 0)
		flags |= MNT_UPDATE;

	switch (mnttype) {
	case MOUNT_UFS:
		if (mntopts)
			getufsopts(mntopts, &flags);
		if (options)
			getufsopts(options, &flags);
		args.fspec = spec;
		args.export.ex_root = DEFAULT_ROOTUID;
		if (flags & MNT_RDONLY)
			args.export.ex_flags = MNT_EXRDONLY;
		else
			args.export.ex_flags = 0;
		argp = (caddr_t)&args;
		break;

	case MOUNT_MFS:
	case MOUNT_NFS:
	default:
		argv[0] = mntname;
		argc = 1;
		if (flags) {
			argv[argc++] = "-F";
			sprintf(flagval, "%d", flags);
			argv[argc++] = flagval;
		}
		if (mntopts)
			argc += getexecopts(mntopts, &argv[argc]);
		if (options)
			argc += getexecopts(options, &argv[argc]);
		argv[argc++] = spec;
		argv[argc++] = name;
		argv[argc++] = NULL;
		sprintf(execname, "%s/mount_%s", _PATH_EXECDIR, mntname);
		if (verbose) {
			(void)printf("exec: %s", execname);
			for (i = 1; i < argc - 1; i++)
				(void)printf(" %s", argv[i]);
			(void)printf("\n");
		}
		if (debug)
			break;
		if (pid = vfork()) {
			if (pid == -1) {
				warn("vfork starting file system");
				return (1);
			}
			if (waitpid(pid, &status, 0) != -1 &&
			    WIFEXITED(status) &&
			    WEXITSTATUS(status) != 0)
				return (WEXITSTATUS(status));
			spec = mntname;
			goto out;
		}
		execv(execname, argv);
		err(1, "cannot exec %s for %s", execname, name);
		/* NOTREACHED */

	}
	if (!debug && mount(mnttype, name, flags, argp)) {
		(void) fprintf(stderr, "%s on %s: ", spec, name);
		switch (errno) {
		case EMFILE:
			(void) fprintf(stderr, "Mount table full\n");
			break;
		case EINVAL:
			if (flags & MNT_UPDATE)
				(void) fprintf(stderr, "Specified device %s\n",
					"does not match mounted device");
			else if (mnttype == MOUNT_UFS)
				(void) fprintf(stderr, "Bogus super block\n");
			else
				perror((char *)NULL);
			break;
		default:
			perror((char *)NULL);
			break;
		}
		return (1);
	}

out:
	if (verbose)
		prmount(spec, name, flags);

	return (0);
}

static void
prmount(spec, name, flags)
	char *spec, *name;
	int flags;
{
	int first;

	(void)printf("%s on %s", spec, name);
	if (!(flags & MNT_VISFLAGMASK)) {
		(void)printf("\n");
		return;
	}
	first = 0;
#define	PR(msg)	(void)printf("%s%s", !first++ ? " (" : ", ", msg)
	if (flags & MNT_RDONLY)
		PR("read-only");
	if (flags & MNT_NOEXEC)
		PR("noexec");
	if (flags & MNT_NOSUID)
		PR("nosuid");
	if (flags & MNT_NODEV)
		PR("nodev");
	if (flags & MNT_SYNCHRONOUS)
		PR("synchronous");
	if (flags & MNT_ASYNC)
		PR("asynchronous");
	if (flags & MNT_QUOTA)
		PR("with quotas");
	if (flags & MNT_LOCAL)
		PR("local");
	if (flags & MNT_UNION)
		PR("union");
	if (flags & MNT_EXPORTED)
		PR("NFS exported");
	(void)printf(")\n");
}

static int
getmnttype(fstype)
	char *fstype;
{

	mntname = fstype;
	if (!strcmp(fstype, "ufs"))
		return (MOUNT_UFS);
	return (0);
}

static void
usage()
{

	(void) fprintf(stderr,
		"usage:\n  mount %s %s\n  mount %s\n  mount %s\n",
		"[ -frwu ] [ -t ufs | external_type ]",
		"[ -o options ] special node",
		"[ -afrwu ] [ -t ufs | external_type ]",
		"[ -frwu ] special | node");
	exit(1);
}

static void
getstdopts(options, flagp)
	char *options;
	int *flagp;
{
	char *opt;
	int negative;
	char optbuf[BUFSIZ];

	(void)strcpy(optbuf, options);
	for (opt = strtok(optbuf, ","); opt; opt = strtok((char *)NULL, ",")) {
		if (opt[0] == '-')
			continue;
		if (opt[0] == 'n' && opt[1] == 'o') {
			negative++;
			opt += 2;
		} else {
			negative = 0;
		}
		if (!negative && !strcasecmp(opt, FSTAB_RO)) {
			*flagp |= MNT_RDONLY;
			continue;
		}
		if (!negative && !strcasecmp(opt, FSTAB_RW)) {
			*flagp &= ~MNT_RDONLY;
			continue;
		}
		if (!strcasecmp(opt, "exec")) {
			if (negative)
				*flagp |= MNT_NOEXEC;
			else
				*flagp &= ~MNT_NOEXEC;
			continue;
		}
		if (!strcasecmp(opt, "suid")) {
			if (negative)
				*flagp |= MNT_NOSUID;
			else
				*flagp &= ~MNT_NOSUID;
			continue;
		}
		if (!strcasecmp(opt, "dev")) {
			if (negative)
				*flagp |= MNT_NODEV;
			else
				*flagp &= ~MNT_NODEV;
			continue;
		}
		if (!strcasecmp(opt, "synchronous")) {
			if (!negative)
				*flagp |= MNT_SYNCHRONOUS;
			else
				*flagp &= ~MNT_SYNCHRONOUS;
			continue;
		}
		if (!strcasecmp(opt, "asynchronous")) {
			if (!negative)
				*flagp |= MNT_ASYNC;
			else
				*flagp &= ~MNT_ASYNC;
			continue;
		}
		if (!strcasecmp(opt, "union")) {
			if (!negative)
				*flagp |= MNT_UNION;
			else
				*flagp &= ~MNT_UNION;
			continue;
		}
		(void) fprintf(stderr, "mount: %s: unknown option\n", opt);
	}
}

/* ARGSUSED */
static void
getufsopts(options, flagp)
	char *options;
	int *flagp;
{

	return;
}

static int
getexecopts(options, argv)
	char *options;
	char **argv;
{
	int argc = 0;
	char *opt;

	for (opt = strtok(options, ","); opt; opt = strtok((char *)NULL, ",")) {
		if (opt[0] != '-')
			continue;
		argv[argc++] = opt;
		if (opt[2] == '\0' || opt[2] != '=')
			continue;
		opt[2] = '\0';
		argv[argc++] = &opt[3];
	}
	return (argc);
}

static struct statfs *
getmntpt(name)
	char *name;
{
	long mntsize;
	long i;
	struct statfs *mntbuf;

	mntsize = getmntinfo(&mntbuf, MNT_NOWAIT);
	for (i = 0; i < mntsize; i++) {
		if (!strcmp(mntbuf[i].f_mntfromname, name) ||
		    !strcmp(mntbuf[i].f_mntonname, name))
			return (&mntbuf[i]);
	}
	return ((struct statfs *)0);
}

static int skipvfs;

static int
badvfstype(vfstype, vfslist)
	int vfstype;
	char **vfslist;
{

	if (vfslist == 0)
		return (0);
	while (*vfslist) {
		if (vfstype == getmnttype(*vfslist))
			return (skipvfs);
		vfslist++;
	}
	return (!skipvfs);
}

static int
badvfsname(vfsname, vfslist)
	char *vfsname;
	char **vfslist;
{

	if (vfslist == 0)
		return (0);
	while (*vfslist) {
		if (strcmp(vfsname, *vfslist) == 0)
			return (skipvfs);
		vfslist++;
	}
	return (!skipvfs);
}

static char **
makevfslist(fslist)
	char *fslist;
{
	char **av, *nextcp;
	int i;

	if (fslist == NULL)
		return (NULL);
	if (fslist[0] == 'n' && fslist[1] == 'o') {
		fslist += 2;
		skipvfs = 1;
	}
	for (i = 0, nextcp = fslist; *nextcp; nextcp++)
		if (*nextcp == ',')
			i++;
	av = (char **)malloc((size_t)(i+2) * sizeof(char *));
	if (av == NULL)
		return (NULL);
	nextcp = fslist;
	i = 0;
	av[i++] = nextcp;
	while (nextcp = index(nextcp, ',')) {
		*nextcp++ = '\0';
		av[i++] = nextcp;
	}
	av[i++] = 0;
	return (av);
}

/*
 * Copyright (c) 1983 Eric P. Allman
 * Copyright (c) 1988, 1993
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
 *
 *	@(#)conf.h	8.10 (Berkeley) 07/20/93
 */

/*
**  CONF.H -- All user-configurable parameters for sendmail
*/

# include <sys/param.h>
# include <sys/stat.h>
# include <fcntl.h>

/**********************************************************************
**  Table sizes, etc....
**	There shouldn't be much need to change these....
**********************************************************************/

# define MAXLINE	2048		/* max line length */
# define MAXNAME	256		/* max length of a name */
# define MAXPV		40		/* max # of parms to mailers */
# define MAXATOM	200		/* max atoms per address */
# define MAXMAILERS	25		/* maximum mailers known to system */
# define MAXRWSETS	100		/* max # of sets of rewriting rules */
# define MAXPRIORITIES	25		/* max values for Precedence: field */
# define MAXMXHOSTS	20		/* max # of MX records */
# define SMTPLINELIM	990		/* maximum SMTP line length */
# define MAXKEY		128		/* maximum size of a database key */
# define MEMCHUNKSIZE	1024		/* chunk size for memory allocation */
# define MAXUSERENVIRON	100		/* max envars saved, must be >= 3 */
# define MAXIPADDR	16		/* max # of IP addrs for this host */
# define MAXALIASDB	12		/* max # of alias databases */
# define PSBUFSIZE	(MAXLINE + MAXATOM)	/* size of prescan buffer */

# ifndef QUEUESIZE
# define QUEUESIZE	1000		/* max # of jobs per queue run */
# endif

/**********************************************************************
**  Compilation options.
**
**	#define these if they are available; comment them out otherwise.
**********************************************************************/

# define LOG		1	/* enable logging */
# define UGLYUUCP	1	/* output ugly UUCP From lines */
# define NETINET	1	/* include internet support */
# define SETPROCTITLE	1	/* munge argv to display current status */
# define NAMED_BIND	1	/* use Berkeley Internet Domain Server */
# define MATCHGECOS	1	/* match user names from gecos field */
# define XDEBUG		1	/* enable extended debugging */

# ifdef NEWDB
# define USERDB		1	/* look in user database (requires NEWDB) */
# endif

/**********************************************************************
**  Operating system configuration.
**
**	Unless you are porting to a new OS, you shouldn't have to
**	change these.
**********************************************************************/

/*
**  Per-Operating System defines
*/

/*
**  HP-UX -- tested for 8.07
*/

# ifdef __hpux
# define SYSTEM5	1	/* include all the System V defines */
# define HASSETREUID	1	/* have setreuid(2) call */
# define setreuid(r, e)		setresuid(r, e, -1)	
# define LA_TYPE	LA_FLOAT
# define _PATH_UNIX	"/hp-ux"
# endif

/*
**  IBM AIX 3.x -- actually tested for 3.2.3
*/

# ifdef _AIX3
# define LOCKF		1	/* use System V lockf instead of flock */
# define FORK		fork	/* no vfork primitive available */
# endif

/*
**  Silicon Graphics IRIX
**
**	Compiles on 4.0.1.
*/

# ifdef IRIX
# define HASSETREUID	1	/* have setreuid(2) call */
# define FORK		fork	/* no vfork primitive available */
# define setpgrp	BSDsetpgrp
# define GIDSET_T	gid_t
# endif


/*
**  SunOS
*/

#if defined(sun) && !defined(BSD)

# define LA_TYPE	LA_INT

# ifdef SOLARIS
			/* Solaris 2.x (a.k.a. SunOS 5.x) */
#  define SYSTEM5	1	/* use System V definitions */
#  include <sys/time.h>
#  define _PATH_UNIX	"/kernel/unix"
#  ifndef _PATH_SENDMAILCF
#   define _PATH_SENDMAILCF	"/etc/mail/sendmail.cf"
#   define _PATH_SENDMAILPID	"/etc/mail/sendmail.pid"
#  endif

# else
			/* SunOS 4.1.x */
#  define HASSTATFS	1	/* has the statfs(2) syscall */
#  define HASSETREUID	1	/* have setreuid(2) call */
#  include <vfork.h>

# endif
#endif

/*
**  Digital Ultrix 4.2A or 4.3
*/

#ifdef ultrix
# define HASSTATFS	1	/* has the statfs(2) syscall */
# define HASSETREUID	1	/* have setreuid(2) call */
# define HASSETENV	1	/* has setenv(3) call */
# define HASUNSETENV	1	/* has unsetenv(3) call */
# define LA_TYPE	LA_INT
# define LA_AVENRUN	"avenrun"
#endif

/*
**  OSF/1 (tested on Alpha)
*/

#ifdef __osf__
# define HASSETENV	1	/* has setenv(3) call */
# define HASUNSETENV	1	/* has unsetenv(3) call */
# define HASSETREUID	1	/* have setreuid(2) call */
# define LA_TYPE	LA_INT
# define LA_AVENRUN	"avenrun"
#endif

/*
**  NeXTstep
*/

#ifdef __NeXT__
# define sleep		sleepX
# define LA_TYPE	LA_ZERO
#endif

/*
**  4.4 BSD
*/

#ifdef BSD4_4
# include <sys/cdefs.h>
# define ERRLIST_PREDEFINED	/* don't declare sys_errlist */
#endif

/*
**  SCO Unix
*/

#ifdef _SCO_unix_
# define SYSTEM5	1	/* include all the System V defines */
# define FORK		fork
# define MAXPATHLEN	PATHSIZE
# define LA_TYPE	LA_ZERO
#endif

/*
**  ConvexOS 11.0 and later
*/

#ifdef _CONVEX_SOURCE
# define SYSTEM5	1	/* include all the System V defines */
# define HASSTATFS	1	/* has the statfs(2) syscall */
# define HASSETSID	1	/* has POSIX setsid(2) call */
# define HASINITGROUPS	1	/* has initgroups(2) call */
# define HASGETDTABLESIZE 1	/* we have getdtablesize(2) call */
# define HASSETREUID	1	/* have setreuid(2) call */
# define HASSETENV	1	/* has setenv(3) call */
# define LA_TYPE	LA_FLOAT
#endif

/*
**  RISC/os 4.51
**
**	Untested...
*/

#ifdef RISCOS
# define HASSETENV	1	/* has setenv(3) call */
# define HASUNSETENV	1	/* has unsetenv(3) call */
# define LA_TYPE	LA_INT
# define LA_AVENRUN	"avenrun"
# define _PATH_UNIX	"/unix"
#endif

/**********************************************************************
**  End of Per-Operating System defines
**********************************************************************/

/**********************************************************************
**  More general defines
**********************************************************************/

/* general BSD defines */
#ifdef BSD
# define HASSETENV	1	/* has setenv(3) call */
# define HASUNSETENV	1	/* has unsetenv(3) call */
# define HASGETDTABLESIZE 1	/* we have getdtablesize(2) call */
# define HASSETREUID	1	/* have setreuid(2) call */
# ifndef LA_TYPE
#  define LA_TYPE	LA_SUBR
# endif
#endif

/* general System V defines */
# ifdef SYSTEM5
# define LOCKF		1	/* use System V locking instead of flock */
# define HASUNAME	1	/* use System V uname(2) system call */
# define HASUSTAT	1	/* use System V ustat(2) syscall */
# ifndef LA_TYPE
#  define LA_TYPE	LA_INT
# endif
# define bcopy(s, d, l)		(memmove((d), (s), (l)))
# define bzero(d, l)		(memset((d), '\0', (l)))
# define bcmp(s, d, l)		(memcmp((s), (d), (l)))
# endif

/* general "standard C" defines */
#if defined(__STDC__) || defined(SYSTEM5)
# define HASSETVBUF	1	/* we have setvbuf(3) in libc */
#endif

/* general POSIX defines */
#ifdef _POSIX_VERSION
# define HASSETSID	1	/* has setsid(2) call */
# define HASWAITPID	1	/* has waitpid(2) call */
#endif

/*
**  Due to a "feature" in some operating systems such as Ultrix 4.3 and
**  HPUX 8.0, if you receive a "No route to host" message (ICMP message
**  ICMP_UNREACH_HOST) on _any_ connection, all connections to that host
**  are closed.  Some firewalls return this error if you try to connect
**  to the IDENT port (113), so you can't receive email from these hosts
**  on these systems.  The firewall really should use a more specific
**  message such as ICMP_UNREACH_PROTOCOL or _PORT or _NET_PROHIB.
*/

#if !defined(ultrix) && !defined(__hpux)
# define IDENTPROTO	1	/* use IDENT proto (RFC 1413) */
#endif

/*
**  If no type for argument two of getgroups call is defined, assume
**  it's an integer -- unfortunately, there seem to be several choices
**  here.
*/

#ifndef GIDSET_T
# define GIDSET_T	int
#endif

/*
**  Remaining definitions should never have to be changed.  They are
**  primarily to provide back compatibility for older systems -- for
**  example, it includes some POSIX compatibility definitions
*/

/* System 5 compatibility */
#ifndef S_ISREG
#define S_ISREG(foo)	((foo & S_IFREG) == S_IFREG)
#endif
#ifndef S_IWGRP
#define S_IWGRP		020
#endif
#ifndef S_IWOTH
#define S_IWOTH		002
#endif

/*
**  Older systems don't have this error code -- it should be in
**  /usr/include/sysexits.h.
*/

# ifndef EX_CONFIG
# define EX_CONFIG	78	/* configuration error */
# endif

#ifndef __P
# include "cdefs.h"
#endif

/*
**  Do some required dependencies
*/

#if defined(NETINET) || defined(NETISO)
# define SMTP		1	/* enable user and server SMTP */
# define QUEUE		1	/* enable queueing */
# define DAEMON		1	/* include the daemon (requires IPC & SMTP) */
#endif


/*
**  Arrange to use either varargs or stdargs
*/

# ifdef __STDC__

# include <stdarg.h>

# define VA_LOCAL_DECL	va_list ap;
# define VA_START(f)	va_start(ap, f)
# define VA_END		va_end(ap)

# else

# include <varargs.h>

# define VA_LOCAL_DECL	va_list ap;
# define VA_START(f)	va_start(ap)
# define VA_END		va_end(ap)

# endif

#ifdef HASUNAME
# include <sys/utsname.h>
# ifdef newstr
#  undef newstr
# endif
#else /* ! HASUNAME */
# define NODE_LENGTH 32
struct utsname
{
	char nodename[NODE_LENGTH+1];
};
#endif /* HASUNAME */

#if !defined(MAXHOSTNAMELEN) && !defined(_SCO_unix_)
# define MAXHOSTNAMELEN	256
#endif

#if !defined(SIGCHLD) && defined(SIGCLD)
# define SIGCHLD	SIGCLD
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO	0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO	1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO	2
#endif

#ifdef LOCKF
#define LOCK_SH		0x01	/* shared lock */
#define LOCK_EX		0x02	/* exclusive lock */
#define LOCK_NB		0x04	/* non-blocking lock */
#define LOCK_UN		0x08	/* unlock */

#else

# include <sys/file.h>

#endif

/*
**  Size of tobuf (deliver.c)
**	Tweak this to match your syslog implementation.  It will have to
**	allow for the extra information printed.
*/

#ifndef TOBUFSIZE
# define TOBUFSIZE (1024 - 256)
#endif

/* fork routine -- set above using #ifdef _osname_ or in Makefile */
# ifndef FORK
# define FORK		vfork		/* function to call to fork mailer */
# endif

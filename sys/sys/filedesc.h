/*
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
 *
 *	@(#)filedesc.h	7.1 (Berkeley) 01/10/91
 */

#ifndef _FILEDESC_
/*
 * This structure is used for the management of descriptors.
 * It may be shared by multiple threads.
 *
 * A process is initially started out with NDFILE worth of
 * descriptors, selected to be enough for typical applications
 * based on the historic limit of 20 open files. Additional
 * descriptors may be allocated up to a system defined limit
 * defined by the global variable nofile; the initial value
 * of nofile is set to NOFILE. The initial expansion is set to
 * NDEXTENT; each time it runs out it is doubled until nofile
 * is reached. NDEXTENT should be selected to be the biggest
 * multiple of OFILESIZE (see below) that will fit in a
 * power-of-two sized piece of memory.
 */
#define NDFILE		20
#define NDEXTENT	25

struct filedesc {
	struct	vnode *fd_cdir;		/* current directory */
	struct	vnode *fd_rdir;		/* root directory */
	u_short	fd_cmask;		/* mask for file creation */
	u_short	fd_refcnt;		/* reference count */
	short	fd_lastfile;		/* high-water mark of fd_ofile */
	short	fd_maxfiles;		/* maximum number of open files */
	struct	file *fd_ofile[NDFILE];	/* file structures for open files */
	struct	file **fd_moreofiles;	/* the rest of the open files */
	char	fd_ofileflags[NDFILE];	/* per-process open file flags */
	char	*fd_moreofileflags;	/* the rest of the open file flags */
	long	fd_spare;		/* unused to round up to power of two */
};

/*
 * Per-process open flags.
 */
#define	UF_EXCLOSE 	0x01		/* auto-close on exec */
#define	UF_MAPPED 	0x02		/* mapped from device */

/*
 * Data structure access macros.
 */
#if !defined(vax) && !defined(tahoe)
#define OFILE(fd, indx) ((indx) < NDFILE ? \
	(fd)->fd_ofile[indx] : \
	(fd)->fd_moreofiles[(indx) - NDFILE])
#define OFILEFLAGS(fd, indx) ((indx) < NDFILE ? \
	(fd)->fd_ofileflags[indx] : \
	(fd)->fd_moreofileflags[(indx) - NDFILE])
#define OFILESIZE (sizeof(struct file *) + sizeof(char))
#else
/* PCC cannot handle above as lvalues */
struct file **ofilefunc();
char *ofileflagsfunc();
#define OFILE(fd, indx) *ofilefunc(fd, indx)
#define OFILEFLAGS(fd, indx) *ofileflagsfunc(fd, indx)
#define OFILESIZE (sizeof(struct file *) + sizeof(char))
#endif

/*
 * Kernel global variables and routines.
 */
extern struct filedesc *fddup();
extern int nofile;
#endif _FILEDESC_

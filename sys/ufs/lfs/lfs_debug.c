/*
 * Copyright (c) 1989, 1991 The Regents of the University of California.
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
 *	@(#)lfs_debug.c	5.2 (Berkeley) 10/02/91
 */

#ifdef LOGFS
#include "param.h"
#include "namei.h"
#include "vnode.h"
#include "../ufs/quota.h"
#include "../ufs/inode.h"
#include "lfs.h"

void 
dump_super(lfsp)
	LFS *lfsp;
{
	int i;

	(void)printf("%s%lx\t%s%lx\t%s%d\t%s%d\n",
		"magic    ", lfsp->lfs_magic,
		"version  ", lfsp->lfs_version,
		"size     ", lfsp->lfs_size,
		"ssize    ", lfsp->lfs_ssize);
	(void)printf("%s%d\t%s%d\t%s%d\t%s%d\n",
		"dsize    ", lfsp->lfs_dsize,
		"bsize    ", lfsp->lfs_bsize,
		"fsize    ", lfsp->lfs_fsize,
		"frag     ", lfsp->lfs_frag);

	(void)printf("%s%d\t%s%d\t%s%d\t%s%d\n",
		"minfree  ", lfsp->lfs_minfree,
		"inopb    ", lfsp->lfs_inopb,
		"ifpb     ", lfsp->lfs_ifpb,
		"nindir   ", lfsp->lfs_nindir);

	(void)printf("%s%d\t%s%d\t%s%d\n",
		"nseg     ", lfsp->lfs_nseg,
		"nspf     ", lfsp->lfs_nspf,
		"segtabsz ", lfsp->lfs_segtabsz);

	(void)printf("%s%lx\t%s%d\t%s%lx\t%s%d\n",
		"segmask  ", lfsp->lfs_segmask,
		"segshift ", lfsp->lfs_segshift,
		"bmask    ", lfsp->lfs_bmask,
		"bshift   ", lfsp->lfs_bshift);

	(void)printf("%s%lx\t%s%d\t%s%lx\t%s%d\n",
		"ffmask   ", lfsp->lfs_ffmask,
		"ffshift  ", lfsp->lfs_ffshift,
		"fbmask   ", lfsp->lfs_fbmask,
		"fbshift  ", lfsp->lfs_fbshift);

	(void)printf("%s%d\t%s%lx\n", 
		"fsbtodb  ", lfsp->lfs_fsbtodb,
		"cksum    ", lfsp->lfs_cksum);

	(void)printf("Superblock disk addresses:");
	for (i = 0; i < LFS_MAXNUMSB; i++)
		(void)printf(" %lx", lfsp->lfs_sboffs[i]);
	(void)printf("\n");

	(void)printf("Checkpoint Info\n");
	(void)printf("%s%d\t%s%lx\t%s%d\n",
		"free     ", lfsp->lfs_free,
		"idaddr   ", lfsp->lfs_idaddr,
		"ifile    ", lfsp->lfs_ifile);
	(void)printf("%s%lx\t%s%d\t%s%lx\t%s%lx\n",
		"bfree    ", lfsp->lfs_bfree,
		"nfiles   ", lfsp->lfs_nfiles,
		"lastseg  ", lfsp->lfs_lastseg,
		"nextseg  ", lfsp->lfs_nextseg);
	(void)printf("tstamp   %lx\n", lfsp->lfs_tstamp);
}

dump_dinode(dip)
	DINODE *dip;
{
	int i;

	(void)printf("%s%d\t%s%d\t%s%d\t%s%d\t%s%d\n",
		"mode  ", dip->di_mode,
		"nlink ", dip->di_nlink,
		"uid   ", dip->di_uid,
		"gid   ", dip->di_gid,
		"size  ", dip->di_size);
	(void)printf("inum  %d\n", dip->di_inum);
	(void)printf("Direct Addresses\n");
	for (i = 0; i < NDADDR; i++) {
		(void)printf("\t%lx", dip->di_db[i]);
		if ((i % 6) == 5)
			(void)printf("\n");
	}
	for (i = 0; i < NIADDR; i++)
		(void)printf("\t%lx", dip->di_ib[i]);
	(void)printf("\n");
}

lfs_print_inumber(vp)
	VNODE *vp;
{
	(void)printf("%d\n", VTOI(vp)->i_number);
}

void
lfs_spin()
{
	u_long i, j;

	for (i = 0; i < 10; ++i)
		for (j = 0; j < 1000000; ++j);
}
#endif /* LOGFS */

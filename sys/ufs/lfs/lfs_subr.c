/*
 * Copyright (c) 1991 Regents of the University of California.
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
 *	@(#)lfs_subr.c	7.3 (Berkeley) 11/05/91
 */

#include <sys/param.h>
#include <sys/namei.h>
#include <sys/vnode.h>
#include <sys/buf.h>

#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>

#include <ufs/lfs/lfs.h>
#include <ufs/lfs/lfs_extern.h>

/*
 * Return buffer with the contents of block "offset" from the beginning of
 * directory "ip".  If "res" is non-zero, fill it in with a pointer to the
 * remaining space in the directory.
 */
int
lfs_blkatoff(vp, offset, res, bpp)
	struct vnode *vp;
	off_t offset;
	char **res;
	struct buf **bpp;
{
	register struct lfs *fs;
	struct inode *ip;
	struct buf *bp;
	daddr_t lbn;
	int bsize, error;

	ip = VTOI(vp);
	fs = ip->i_lfs;
	lbn = lblkno(fs, offset);
	bsize = blksize(fs);

	*bpp = NULL;
	if (error = bread(vp, lbn, bsize, NOCRED, &bp)) {
		brelse(bp);
		return (error);
	}
	if (res)
		*res = bp->b_un.b_addr + blkoff(fs, offset);
	*bpp = bp;
	return (0);
}

/* Return the current version number for a specific inode. */
u_long
lfs_getversion(fs, ino)
	struct lfs *fs;
	ino_t ino;
{
	BUF *bp;
	IFILE *ifp;
	u_long version;

	/*
	 * Read the appropriate block from the ifile.  Return the
	 * version number.
	 */
	LFS_IENTRY(ifp, fs, ino, bp);
	version = ifp->if_version;
	brelse(bp);
	return (version);
}

/* Search a block for a specific dinode. */
DINODE *
lfs_ifind(fs, ino, page)
	struct lfs *fs;
	ino_t ino;
	void *page;
{
	register DINODE *dip;
	register int cnt;

#ifdef ALLOCPRINT
	printf("lfs_ifind: inode %d\n", ino);
#endif
	dip = page;
	for (cnt = INOPB(fs); cnt--; ++dip)
		if (dip->di_inum == ino)
			return (dip);

	panic("lfs_ifind: dinode %u not found", ino);
	/* NOTREACHED */
}

/* Set values in the ifile for the inode. */
void
lfs_iset(ip, daddr, atime)
	INODE *ip;
	daddr_t daddr;
	time_t atime;
{
	BUF *bp;
	IFILE *ifp;
	struct lfs *fs;
	ino_t ino;

#ifdef ALLOCPRINT
	printf("lfs_iset: setting ino %d daddr %lx time %lx\n",
	    ip->i_number, daddr, atime);
#endif

	fs = ip->i_lfs;
	ino = ip->i_number;
	LFS_IENTRY(ifp, fs, ino, bp);

	ifp->if_daddr = daddr;
	ifp->if_st_atime = atime;
	lfs_bwrite(bp);
}

/* Translate an inode number to a disk address. */
daddr_t
lfs_itod(fs, ino)
	struct lfs *fs;
	ino_t ino;
{
	BUF *bp;
	IFILE *ifp;
	daddr_t iaddr;

	/* Read the appropriate block from the ifile. */
	LFS_IENTRY(ifp, fs, ino, bp);

	if (ifp->if_daddr == LFS_UNUSED_DADDR)
		panic("itod: unused disk address");
	iaddr = ifp->if_daddr;
	brelse(bp);
	return (iaddr);
}

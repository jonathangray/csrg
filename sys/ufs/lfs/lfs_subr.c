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
 *	@(#)lfs_subr.c	7.12 (Berkeley) 06/23/92
 */

#include <sys/param.h>
#include <sys/namei.h>
#include <sys/vnode.h>
#include <sys/buf.h>
#include <sys/mount.h>

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
lfs_blkatoff (ap)
	struct vop_blkatoff_args *ap;
{
	register struct lfs *fs;
	struct inode *ip;
	struct buf *bp;
	daddr_t lbn;
	int bsize, error;

	ip = VTOI(ap->a_vp);
	fs = ip->i_lfs;
	lbn = lblkno(fs, ap->a_offset);
	bsize = blksize(fs);

	*ap->a_bpp = NULL;
	if (error = bread(ap->a_vp, lbn, bsize, NOCRED, &bp)) {
		brelse(bp);
		return (error);
	}
	if (ap->a_res)
		*ap->a_res = bp->b_un.b_addr + blkoff(fs, ap->a_offset);
	*ap->a_bpp = bp;
	return (0);
}

int
lfs_mntinvalbuf(mp)
	struct mount *mp;
{
	struct vnode *vp;
	int dirty;

	dirty = 0;
	if ((mp->mnt_flag & MNT_MPBUSY) == 0)
		panic("lfs_mntinvalbuf: not busy");
loop:
	for (vp = mp->mnt_mounth; vp; vp = vp->v_mountf) {
		if (VTOI(vp)->i_number == LFS_IFILE_INUM)
			continue;
		if (vget(vp))
			goto loop;
		dirty += lfs_vinvalbuf(vp);
		vput(vp);
		if (vp->v_mount != mp)
			goto loop;
	}
	return (dirty);
}

/*
 * For LFS, we need to do two passes.  First we need to wait on any dirty and
 * busy buffers.  Once we've guaranteed that all the buffers are unbusy, we
 * can do the segment write.  Then we need to go through and invalidate all
 * the buffers on the clean list.
 */
int
lfs_vinvalbuf(vp)
	register struct vnode *vp;
{
	register struct buf *bp;
	struct buf *nbp, *blist;
	int s, dirty = 0;

loop:	for (bp = vp->v_dirtyblkhd; bp; bp = nbp) {
		nbp = bp->b_blockf;
		s = splbio();
		if (bp->b_flags & B_BUSY) {
			bp->b_flags |= B_WANTED;
			sleep((caddr_t)bp, PRIBIO + 1);
			splx(s);
			goto loop;
		}
		bremfree(bp);
		splx(s);
		dirty++;
		brelse(bp);
	}
	if (dirty)
		lfs_segwrite(vp->v_mount, 0);

	/* Remove blocks from the clean list. */
	for (bp = vp->v_cleanblkhd; bp; bp = nbp) {
		nbp = bp->b_blockf;
		bremfree(bp);
		bp->b_flags |= B_INVAL;
		brelse(bp);
	}

	if (vp->v_dirtyblkhd || vp->v_cleanblkhd)
		panic("lfs_vinvalbuf: flush failed");
	return (dirty);
}

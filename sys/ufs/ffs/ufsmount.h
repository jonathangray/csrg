/*
 * Copyright (c) 1982, 1986, 1989 Regents of the University of California.
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
 *	@(#)ufsmount.h	7.10 (Berkeley) 11/01/91
 */

/*
 * The root inode is the root of the file system.  Inode 0 can't be used for
 * normal purposes and historically bad blocks were linked to inode 1, thus
 * the root inode is 2. (inode 1 is no longer used for this purpose, however
 * numerous dump tapes make this assumption, so we are stuck with it).
 */
#define	ROOTINO	((ino_t)2)

struct buf;
struct inode;
struct nameidata;
struct timeval;
struct ucred;
struct uio;
struct vnode;

/* This structure describes the UFS specific mount structure data. */
struct ufsmount {
	struct	mount *um_mountp;		/* filesystem vfs structure */
	dev_t	um_dev;				/* device mounted */
	struct	vnode *um_devvp;		/* block device mounted vnode */
	union {					/* pointer to superblock */
		struct	lfs *lfs;		/* LFS */
		struct	fs *fs;			/* FFS */
	} ufsmount_u;
#define	um_fs	ufsmount_u.fs
#define	um_lfs	ufsmount_u.lfs
	struct	vnode *um_quotas[MAXQUOTAS];	/* pointer to quota files */
	struct	ucred *um_cred[MAXQUOTAS];	/* quota file access cred */
	time_t	um_btime[MAXQUOTAS];		/* block quota time limit */
	time_t	um_itime[MAXQUOTAS];		/* inode quota time limit */
	char	um_qflags[MAXQUOTAS];		/* quota specific flags */

	/*
	 * The following is the inode switch.  It is intended to provide
	 * the interface between the Unix File System semantics and the
	 * on-disk allocation, layout and I/O.
	 */
	int	(*um_blkatoff) __P((struct inode *ip,
		    off_t offset, char **res, struct buf **bpp));
	int	(*um_write) __P((struct vnode *vp,
		    struct uio *uio, int ioflag, struct ucred *cred));
	int	(*um_iget) __P((struct inode *pip,
		    ino_t ino, struct inode **ipp));
	int	(*um_ialloc) __P((struct inode *pip,
		    int mode, struct ucred *cred, struct inode **ipp));
	void	(*um_ifree) __P((struct inode *pip, ino_t ino, int mode));
	int	(*um_itrunc) __P((struct inode *oip, u_long length, int flags));
	int	(*um_iupdat) __P((struct inode *ip,
		    struct timeval *ta, struct timeval *tm, int waitfor));
	int	(*um_bwrite)		/* XXX changes */
		    __P((struct buf *bp));
	int	(*um_bmap)		/* XXX changes */
		    __P((struct inode *ip, daddr_t bn, daddr_t *bnp));
};
/*
 * Flags describing the state of quotas.
 */
#define	QTF_OPENING	0x01			/* Q_QUOTAON in progress */
#define	QTF_CLOSING	0x02			/* Q_QUOTAOFF in progress */

/* Convert mount ptr to ufsmount ptr. */
#define VFSTOUFS(mp)	((struct ufsmount *)((mp)->mnt_data))

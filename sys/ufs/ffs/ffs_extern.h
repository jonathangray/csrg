/*-
 * Copyright (c) 1991 The Regents of the University of California.
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
 *	@(#)ffs_extern.h	7.6 (Berkeley) 04/21/92
 */

struct buf;
struct fid;
struct fs;
struct inode;
struct mount;
struct nameidata;
struct proc;
struct statfs;
struct timeval;
struct ucred;
struct uio;
struct vnode;

__BEGIN_DECLS
int	ffs_alloc __P((struct inode *,
	    daddr_t, daddr_t, int, struct ucred *, daddr_t *));
int	ffs_balloc __P((struct inode *,
	    daddr_t, int, struct ucred *, struct buf **, int));
int	ffs_blkatoff __P((struct vnode *, off_t, char **, struct buf **));
int	ffs_blkfree __P((struct inode *, daddr_t, long));
daddr_t	ffs_blkpref __P((struct inode *, daddr_t, int, daddr_t *));
int	ffs_bmap __P((struct vnode *, daddr_t, struct vnode **, daddr_t *));
void	ffs_clrblock __P((struct fs *, u_char *, daddr_t));
int	ffs_fhtovp __P((struct mount *, struct fid *, int, struct vnode **));
void	ffs_fragacct __P((struct fs *, int, long [], int));
int	ffs_fsync
	    __P((struct vnode *, int, struct ucred *, int, struct proc *));
int	ffs_inactive __P((struct vnode *, struct proc *));
int	ffs_init __P((void));
int	ffs_isblock __P((struct fs *, u_char *, daddr_t));
int	ffs_mount __P((struct mount *,
	    char *, caddr_t, struct nameidata *, struct proc *));
int	ffs_mountfs __P((struct vnode *, struct mount *, struct proc *));
int	ffs_mountroot __P((void));
int	ffs_read __P((struct vnode *, struct uio *, int, struct ucred *));
int	ffs_realloccg __P((struct inode *,
	    daddr_t, daddr_t, int, int, struct ucred *, struct buf **));
int	ffs_reclaim __P((struct vnode *));
int	ffs_root __P((struct mount *, struct vnode **));
void	ffs_setblock __P((struct fs *, u_char *, daddr_t));
int	ffs_statfs __P((struct mount *, struct statfs *, struct proc *));
int	ffs_sync __P((struct mount *, int));
int	ffs_truncate __P((struct vnode *, off_t, int, struct ucred *));
int	ffs_unmount __P((struct mount *, int, struct proc *));
int	ffs_update
	    __P((struct vnode *, struct timeval *, struct timeval *, int));
int	ffs_valloc __P((struct vnode *, int, struct ucred *, struct vnode **));
void	ffs_vfree __P((struct vnode *, ino_t, int));
int	ffs_vget __P((struct mount *, ino_t, struct vnode **));
int	ffs_vptofh __P((struct vnode *, struct fid *));
int	ffs_write __P((struct vnode *, struct uio *, int, struct ucred *));

int	bwrite();		/* FFS needs a bwrite routine.  XXX */

#ifdef DIAGNOSTIC
void	ffs_checkoverlap __P((struct buf *, struct inode *));
#endif
__END_DECLS

extern int inside[], around[];
extern u_char *fragtbl[];
extern struct vnodeops ffs_vnodeops, ffs_specops;
#ifdef FIFO
extern struct vnodeops ffs_fifoops;
#define FFS_FIFOOPS &ffs_fifoops
#else
#define FFS_FIFOOPS NULL
#endif

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
 *	@(#)lfs_extern.h	5.1 (Berkeley) 09/25/91
 */

struct fid;
struct inode;
struct mount;
struct statfs;
struct proc;
struct ucred;
struct vnode;

daddr_t	 itod __P((LFS *, ino_t));
int	 lfs_balloc __P((LFS *, VNODE *, daddr_t, int, BUF **));
int	 lfs_bmap __P((struct inode *, daddr_t, daddr_t *));
void	 lfs_bwrite __P((struct buf *));
int	 lfs_fhtovp __P((struct mount *, struct fid *, struct vnode **));
u_long	 lfs_getversion __P((LFS *fs, ino_t));
ino_t	 lfs_ialloc __P((LFS *, struct inode *, struct inode **,
	     struct ucred *));
IFILE	*lfs_ientry __P((LFS *, ino_t));
struct dinode *
	 lfs_ifind __P((LFS *, ino_t, void *));
void	 lfs_ifree __P((struct inode *));
int	 lfs_inactive __P((struct vnode *, struct proc *));
int	 lfs_init __P((void));
int	 lfs_lookup __P((struct vnode *, struct nameidata *, struct proc *));
int	 lfs_mount __P((struct mount *, char *, caddr_t, struct nameidata *, struct proc *));
int	 lfs_root __P((struct mount *, struct vnode **));
int	 lfs_statfs __P((struct mount *, struct statfs *, struct proc *));
int	 lfs_sync __P((struct mount *, int));
int	 lfs_unmount __P((struct mount *, int, struct proc *));
int	 lfs_vcreate __P((struct mount *, ino_t, struct vnode **));

#ifdef DEBUG
void	dump_super __P((LFS *));
void	dump_dinode __P((struct dinode *));
void	lfs_print_inumber __P((struct vnode *));
#endif

extern struct vnodeops lfs_vnodeops;

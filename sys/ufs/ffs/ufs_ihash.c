/*
 * Copyright (c) 1982, 1986, 1989, 1991 Regents of the University of California.
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
 *	@(#)ufs_ihash.c	7.2 (Berkeley) 11/05/91
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/namei.h>
#include <sys/vnode.h>

#include <ufs/ufs/quota.h>
#include <ufs/ufs/inode.h>
#include <ufs/ufs/ufs_extern.h>

#define	INOHSZ	512
#if	((INOHSZ & (INOHSZ - 1)) == 0)
#define	INOHASH(dev, ino)	(((dev) + (ino)) & (INOHSZ - 1))
#else
#define	INOHASH(dev, ino)	(((unsigned int)((dev) + (ino))) % INOHSZ)
#endif

static union ihead {
	union  ihead *ih_head[2];
	struct inode *ih_chain[2];
} ihead[INOHSZ];


/*
 * Initialize inode hash table.
 */
void
ufs_ihashinit()
{
	register union ihead *ih;
	register int i;

	for (ih = ihead, i = INOHSZ; --i >= 0; ++ih)
		ih->ih_head[0] = ih->ih_head[1] = ih;
}

/*
 * Use the dev/ino pair to find the incore inode, and return a pointer to it.
 * If it is in core, but locked, wait for it.
 */
struct vnode *
ufs_ihashget(dev, ino)
	/* dev_t */ int dev;
	ino_t ino;
{
	register union ihead *ih;
	register struct inode *ip;
	struct vnode *vp;

	ih = &ihead[INOHASH(dev, ino)];
loop:
	for (ip = ih->ih_chain[0]; ip != (struct inode *)ih; ip = ip->i_forw) {
		if (ino != ip->i_number || dev != ip->i_dev)
			continue;
		if ((ip->i_flag & ILOCKED) != 0) {
			ip->i_flag |= IWANT;
			sleep((caddr_t)ip, PINOD);
			goto loop;
		}
		vp = ITOV(ip);
		if (vget(vp))
			goto loop;
		return (vp);
	}
	return (NULL);
}

/*
 * Insert the inode into the hash table, and return it locked.
 */
void
ufs_ihashins(ip)
	struct inode *ip;
{
	insque(ip, &ihead[INOHASH(ip->i_dev, ip->i_number)]);
	ILOCK(ip);
}

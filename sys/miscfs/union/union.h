/*
 * Copyright (c) 1994 The Regents of the University of California.
 * Copyright (c) 1994 Jan-Simon Pendry.
 * All rights reserved.
 *
 * This code is derived from software donated to Berkeley by
 * Jan-Simon Pendry.
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
 *	@(#)union.h	1.4 (Berkeley) 02/03/94
 */

struct union_args {
	char		*target;	/* Target of loopback  */
};

struct union_mount {
	struct vnode	*um_uppervp;
	struct vnode	*um_lowervp;
	struct ucred	*um_cred;	/* Credentials of user calling mount */
};

/* begin XXX */
#define VT_UNION VT_LOFS
/*#define MOUNT_UNION 15*/
/* end XXX */

#ifdef KERNEL

/*
 * DEFDIRMODE is the mode bits used to create a shadow directory.
 */
#define VRWXMODE (VREAD|VWRITE|VEXEC)
#define VRWMODE (VREAD|VWRITE)
#define UN_DIRMODE ((VRWXMODE)|(VRWXMODE>>3)|(VRWXMODE>>6))
#define UN_FILEMODE ((VRWMODE)|(VRWMODE>>3)|(VRWMODE>>6))

/*
 * A cache of vnode references
 */
struct union_node {
	struct union_node	*un_next;	/* Hash chain */
	struct vnode		*un_vnode;	/* Back pointer */
	struct vnode	        *un_uppervp;	/* overlaying object */
	struct vnode	        *un_lowervp;	/* underlying object */
	struct vnode		*un_dirvp;	/* Parent dir of uppervp */
	char			*un_path;	/* saved component name */
	int			un_flags;
	pid_t			un_pid;
};

#define UN_WANT 0x01
#define UN_LOCKED 0x02

extern int union_allocvp __P((struct vnode **, struct mount *, struct vnode *, struct vnode *, struct componentname *, struct vnode *, struct vnode *));

#define	MOUNTTOUNIONMOUNT(mp) ((struct union_mount *)((mp)->mnt_data))
#define	VTOUNION(vp) ((struct union_node *)(vp)->v_data)
#define	UNIONTOV(un) ((un)->un_vnode)
#define	LOWERVP(vp) (VTOUNION(vp)->un_lowervp)
#define	UPPERVP(vp) (VTOUNION(vp)->un_uppervp)
#define OTHERVP(vp) (UPPERVP(vp) ? UPPERVP(vp) : LOWERVP(vp))

extern int (**union_vnodeop_p)();
extern struct vfsops union_vfsops;
#endif /* KERNEL */
